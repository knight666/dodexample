class Header:
	def __init__(self, filename):
		self.file = open(filename, 'wb+')
		self.tab_length = 0
	
	def close(self):
		self.file.close()
	
	def copyrightNotice(self):
		self.writeLine("Copyright (C) 2014-2015 Quinten Lansu")
		self.newLine()
		self.writeLine("Permission is hereby granted, free of charge, to any person")
		self.writeLine("obtaining a copy of this software and associated documentation")
		self.writeLine("files (the \"Software\"), to deal in the Software without")
		self.writeLine("restriction, including without limitation the rights to use,")
		self.writeLine("copy, modify, merge, publish, distribute, sublicense, and/or")
		self.writeLine("sell copies of the Software, and to permit persons to whom the")
		self.writeLine("Software is furnished to do so, subject to the following")
		self.writeLine("conditions:")
		self.newLine()
		self.writeLine("The above copyright notice and this permission notice shall be")
		self.writeLine("included in all copies or substantial portions of the Software.")
		self.newLine()
		self.writeLine("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,")
		self.writeLine("EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES")
		self.writeLine("OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND")
		self.writeLine("NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT")
		self.writeLine("HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,")
		self.writeLine("WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING")
		self.writeLine("FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR")
		self.writeLine("OTHER DEALINGS IN THE SOFTWARE.")
	
	def indent(self):
		self.tab_length += 1
	
	def outdent(self):
		self.tab_length -= 1
	
	def write(self, line):
		self.file.write(line)
	
	def newLine(self):
		self.file.write('\r\n')
	
	def writeIndentation(self):
		for i in range(0, self.tab_length):
			self.file.write('\t')
	
	def writeLine(self, line):
		self.writeIndentation()
		self.write(line)
		self.newLine()