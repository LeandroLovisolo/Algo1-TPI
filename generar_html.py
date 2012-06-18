
from pygments import highlight
from pygments.lexers import CppLexer
from pygments.formatters import HtmlFormatter

formatter = HtmlFormatter(encoding="utf-8", linenos=True, nobackground=True)

def styles():
	css =  "<style type=\"text/css\">\n"
	css += formatter.get_style_defs('.highlight')
	css += "</style>\n"
	return css

def code():
	highlighted = ""
	for file in ["ojota.cpp", "tipos.h", "atleta.h", "atleta.cpp", "competencia.h",  "competencia.cpp", "jjoo.h", "jjoo.cpp"]:
		highlighted += "<h2>" + file + "</h2>\n"
		f = open(file)
		code = f.read()
		f.close()
		highlighted += highlight(code, CppLexer(encoding="utf-8"), formatter)
	return highlighted

html =  "<!doctype html>\n"
html += "<html>\n"
html += "<head><title>TPI</title></head>\n"
html += "<body>\n"
html += styles()
html += code()
html += "</body>\n"
html += "</html>\n"

f = open("output.html", "wb+")
f.write(html)
f.close()

print "Done"
