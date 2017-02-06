#!/usr/bin/env python3
import subprocess

reportTemplate = ''' 
\\documentclass{article}

\\usepackage{graphicx}
\\usepackage{indentfirst}
\\usepackage{setspace}
\\doublespacing

\\title{Simulation Report}
\\date{%s}
\\author{%s}

\\begin{document}

\\maketitle
\\pagenumbering{gobble}
\\newpage
\\pagenumbering{arabic}

%s

\\end{document}
'''

figureTemplate = '''
\\begin {figure}
\\includegraphics[width=\linewidth]{%s}
\\caption{%s}
\\label{%s}
\\end{figure}
'''

def create_fig(filename, caption, ref=None):
    if ref is None:
        ref = 'fig:%s' % caption
    return figureTemplate % (filename, caption, ref)

def main():

    plottypes = ['density', 'location', 'num-networks', 'network-size']

    reportBody = ''
    for plottype in plottypes:
        subprocess.call(['./plot.py', plottype])
        reportBody += create_fig(plottype + '.png', '', '') + '\n'
    
    report = reportTemplate % ('02-06-2017', 'Eli Stone and Lucas Saldyt', reportBody)
    with open('report.tex', 'w') as reportfile:
        reportfile.write(report)

    subprocess.call(['pdflatex', 'report.tex'])
    subprocess.call(['xdg-open', 'report.pdf'])

if __name__ == '__main__':
    main()
