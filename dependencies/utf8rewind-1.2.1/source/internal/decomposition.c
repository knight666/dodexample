/*
	Copyright (C) 2014-2015 Quinten Lansu

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#include "decomposition.h"

#include "codepoint.h"
#include "database.h"

uint8_t decompose_initialize(DecomposeState* state, StreamState* input, StreamState* output, uint8_t compatibility)
{
	memset(state, 0, sizeof(DecomposeState));

	/* Ensure streams are valid */

	if (input == 0 ||
		output == 0)
	{
		return 0;
	}

	/* Set up streams */

	state->input = input;

	state->output = output;
	memset(state->output, 0, sizeof(StreamState));

	/* Set up codepoint quickcheck property */

	state->property = (compatibility == 1)
		? UnicodeProperty_Normalization_Compatibility_Decompose
		: UnicodeProperty_Normalization_Decompose;

	return 1;
}

uint8_t decompose_execute(DecomposeState* state)
{
	unicode_t* src_codepoint;
	unicode_t* dst_codepoint;
	uint8_t* dst_canonical_combining_class;
	uint8_t* dst_quick_check;
	uint8_t uncached = 1;

	/* Check if input is valid */

	if (state->input == 0)
	{
		return 0;
	}

	/* Set up output */

	state->output->current = 0;
	state->output->index = 0;
	state->output->stable = 1;

	dst_codepoint = state->output->codepoint;
	dst_canonical_combining_class = state->output->canonical_combining_class;
	dst_quick_check = state->output->quick_check;

	/* Check cache for stored sequences */

	if (state->cache_current < state->cache_filled)
	{
		/* Read from cache */

		while (state->cache_current < state->cache_filled)
		{
			if (state->output->current > 0 &&
				state->cache_canonical_combining_class[state->cache_current] == 0)
			{
				/* Sequence ends on next non-starter or end of data */

				break;
			}

			*dst_codepoint++ = state->cache_codepoint[state->cache_current];
			*dst_canonical_combining_class++ = state->cache_canonical_combining_class[state->cache_current];
			*dst_quick_check++ = QuickCheckResult_Yes;

			state->output->current++;
			state->cache_current++;
		}

		/* Check if cache has been emptied */

		if (state->cache_current == state->cache_filled)
		{
			state->cache_current = 0;
			state->cache_filled = 0;
		}

		/* Check for additional input */

		if (state->input->index == state->input->current)
		{
			/* Don't compare canonical combining classes, output will always be stable */

			return state->output->current;
		}
	}

	/* Read next sequence from input */

	if (state->input->index == state->input->current &&
		!stream_read(state->input, state->property))
	{
		/* End of data */

		state->input = 0;

		return 0;
	}

	/* Read from source */

	src_codepoint = state->input->codepoint + state->input->index;

	while (state->input->index < state->input->current)
	{
		if (*src_codepoint <= MAX_BASIC_LATIN)
		{
			/* Basic Latin codepoints are already decomposed */

			if (uncached)
			{
				*dst_codepoint++ = *src_codepoint;
				*dst_canonical_combining_class++ = 0;
				*dst_quick_check++ = QuickCheckResult_Yes;

				state->output->current++;
			}
			else
			{
				state->cache_codepoint[state->cache_filled] = *src_codepoint;
				state->cache_canonical_combining_class[state->cache_filled] = 0;

				state->cache_filled++;
			}
		}
		else if (
			*src_codepoint >= HANGUL_S_FIRST &&
			*src_codepoint <= HANGUL_S_LAST)
		{
			/*
				Hangul decomposition

				Algorithm adapted from Unicode Technical Report #15:
				http://www.unicode.org/reports/tr15/tr15-18.html#Hangul
			*/

			unicode_t s_index = *src_codepoint - HANGUL_S_FIRST;

			if (uncached)
			{
				*dst_codepoint++ = HANGUL_L_FIRST + (s_index / HANGUL_N_COUNT);
				*dst_canonical_combining_class++ = 0;
				*dst_quick_check++ = QuickCheckResult_Yes;

				state->output->current++;
			}
			else
			{
				state->cache_codepoint[state->cache_filled] = HANGUL_L_FIRST + (s_index / HANGUL_N_COUNT);
				state->cache_canonical_combining_class[state->cache_filled] = 0;

				state->cache_filled++;
			}

			/* Store subsequent non-starters in cache */

			uncached = 0;

			state->cache_codepoint[state->cache_filled] = HANGUL_V_FIRST + (s_index % HANGUL_N_COUNT) / HANGUL_T_COUNT;
			state->cache_canonical_combining_class[state->cache_filled] = 0;

			state->cache_filled++;

			if ((s_index % HANGUL_T_COUNT) != 0)
			{
				state->cache_codepoint[state->cache_filled] = HANGUL_T_FIRST + (s_index % HANGUL_T_COUNT);
				state->cache_canonical_combining_class[state->cache_filled] = 0;

				state->cache_filled++;
			}
		}
		else
		{
			/* Use quick check to skip stable codepoints */

			unicode_t decoded_codepoint = *src_codepoint;
			uint8_t decoded_quick_check = database_queryproperty(decoded_codepoint, state->property);
			uint8_t decoded_canonical_combining_class;
			uint8_t decoded_size;

			if (decoded_quick_check != QuickCheckResult_Yes)
			{
				/* Check database for decomposition */

				const char* decomposition = database_querydecomposition(decoded_codepoint, state->property);
				if (decomposition != 0)
				{
					/* Write sequence to output */

					const char* src = decomposition;
					size_t src_size = strlen(decomposition);

					while (src_size > 0)
					{
						decoded_canonical_combining_class = database_queryproperty(decoded_codepoint, state->property);

						/* Decode current codepoint */

						decoded_size = codepoint_read(src, src_size, &decoded_codepoint);
						if (decoded_size == 0)
						{
							break;
						}

						decoded_canonical_combining_class = database_queryproperty(decoded_codepoint, UnicodeProperty_CanonicalCombiningClass);

						/* Check for end of sequence */

						if (uncached &&
							state->output->current > 0 &&
							decoded_canonical_combining_class == 0)
						{
							uncached = 0;
						}

						if (uncached)
						{
							/* Write codepoint to output */

							*dst_codepoint++ = decoded_codepoint;
							*dst_canonical_combining_class++ = decoded_canonical_combining_class;
							*dst_quick_check++ = QuickCheckResult_Yes;

							state->output->current++;
						}
						else
						{
							/* Store in cache */

							state->cache_codepoint[state->cache_filled] = decoded_codepoint;
							state->cache_canonical_combining_class[state->cache_filled] = decoded_canonical_combining_class;

							state->cache_filled++;
						}

						src += decoded_size;
						src_size -= decoded_size;
					}
				}
			}
			else
			{
				decoded_canonical_combining_class = database_queryproperty(decoded_codepoint, UnicodeProperty_CanonicalCombiningClass);

				if (uncached)
				{
					/* Write codepoint to output */

					*dst_codepoint++ = decoded_codepoint;
					*dst_canonical_combining_class++ = decoded_canonical_combining_class;
					*dst_quick_check++ = decoded_quick_check;

					state->output->current++;
				}
				else
				{
					/* Store in cache */

					state->cache_codepoint[state->cache_filled] = decoded_codepoint;
					state->cache_canonical_combining_class[state->cache_filled] = decoded_canonical_combining_class;

					state->cache_filled++;
				}
			}
		}

		src_codepoint++;
		state->input->index++;
	}

	if (state->output->current > 1)
	{
		/* Check if output is stable by comparing canonical combining classes */

		uint8_t i;
		for (i = 1; i < state->output->current; ++i)
		{
			if (state->output->canonical_combining_class[i] < state->output->canonical_combining_class[i - 1])
			{
				state->output->stable = 0;

				break;
			}
		}
	}

	return state->output->current;
}