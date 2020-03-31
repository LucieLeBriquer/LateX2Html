#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string  replace_verb_line(string ligne, int len, string res)
{   
    int verb = ligne.find("\\verb");
    if ((verb <= len) && (verb >= 0))
    { 
        string  sub1 = ligne.substr(0, verb);
        string  sub2 = ligne.substr(verb+6, len);
        int     end = sub2.find("|");
        string sub3 = sub2.substr(0, end);
        string sub4 = sub2.substr(end+1, sub2.length());
        return (replace_verb_line(sub4, len, res + sub1 + "<code class='codeb'>" + sub3 + "</code>"));
    }
    else
    {
        return (res + ligne);
    }
}

void    replace_code(string file)
{
    string name1 = file+".txt";
    string name2 = file+".html";
    ifstream sortie1;
    sortie1.open(name1, ios::in);
    ofstream sortie;
    sortie.open(name2, ios::out | ios::trunc);

    if (sortie1)
    {
        string ligne;
        while (getline(sortie1, ligne))
        {
            size_t  len = ligne.length();
            int     verb = ligne.find("\\verb");
            int     verbatimb = ligne.find("\\begin{verbatim}");
            int     verbatime = ligne.find("\\end{verbatim}");
            if ((verb >=0) && (verb < len-3))
            {
                sortie << replace_verb_line(ligne, len, "") << endl;
            }
            else if ((verbatimb >= 0) && (verbatimb <= len - 4)) 
            {
                sortie << "<pre class='codebb'>" << endl;
            }
            else if ((verbatime >= 0) && (verbatime <= len - 4)) 
            {
                sortie << "</pre>" << endl;
            }
            else
            {
                sortie << ligne << endl;
            }
        }
        sortie1.close();
        sortie.close();
    }
    else
    {
        cout << "erreur lecture" << endl;
    }
}

void     convert_to_html(string file)
{
    ifstream fichier(file+".tex");
    ofstream sortie1(file+".txt");

    if(fichier)
    {
        string ligne;
        while(getline(fichier, ligne))
        {
            size_t  len = ligne.length();
            int     cpt = 0;
            while (cpt < len)
            {
                if (ligne.find("\\begin{itemize}") == cpt)
                {
                    sortie1 << "<ul>" << endl;
                    break;
                }
                else if (ligne.find("\\end{itemize}") == cpt)
                {
                    sortie1 << endl << "</ul>" << endl;
                    break;
                }
                else if (ligne.find("\\item") == cpt)
                {
                    string sub1 = ligne.substr(0, cpt - 1);
                    string sub2 = ligne.substr(cpt + 6, len);
                    sortie1 << sub1 << "<li>" << sub2 << "</li>" << endl;
                    break;
                }
                cpt++;
            }
            if (ligne.find("\\section") == 0)
            {
                string sub1 = ligne.substr(9, len - 10);
                sortie1 << "<h2>" << sub1 << "</h2>" << endl;
            }
            else if (ligne.find("\\subsection") == 0)
            {
                string sub1 = ligne.substr(12, len - 13);
                sortie1 << "<h3>" << sub1 << "</h3>" << endl;
            }
            else if ((ligne.find("\\newpage") == 0) ||
                    (ligne.find("\\begin{itemize}") != -1)|| 
                    (ligne.find("\\item") != -1)|| 
                    (ligne.find("\\end{itemize}") != -1))
            {
            }
            else
            {
                sortie1 << ligne << endl;
            }
        }
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }
    replace_code(file);
}

int main(int argc, char **argv)
{
    string file_tex = argv[argc-1];
    string file = file_tex.substr(0, (file_tex.length() - 4));
    convert_to_html(file);
    return (0);
}
