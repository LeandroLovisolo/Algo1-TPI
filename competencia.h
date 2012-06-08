#ifndef COMPETENCIA_H
#define COMPETENCIA_H

#include "atleta.h"
#include "lista.h"

class Competencia{
    public:

        Competencia();
        Competencia(const Deporte d, const Sexo s, const Lista<Atleta>& participantes);
        Categoria categoria() const;

        Lista<Atleta> participantes() const;
        bool finalizada() const;

        Lista<Atleta> ranking() const;

        Lista<Atleta> lesTocoControlAntidoping() const;
        bool leDioPositivo(const Atleta& a) const;
        void finalizar(const Lista<int>& posiciones, const Lista<pair<int, bool> >& control);
        void linfordChristie(const int ciaNum);
        bool gananLosMasCapaces() const;
        void sancionarTramposos();

        bool operator==(const Competencia& c) const;

        void mostrar(std::ostream& os) const;

        void guardar(std::ostream& os) const;

		void cargar (std::istream& is);
	private:

        Categoria _categoria;
        Lista<Atleta> _participantes;
        bool _finalizada;
        Lista<int> _ranking;
        Lista<pair<int, bool> > _controlAntidoping;
        Atleta atletaConCia(const int ciaNumber) const {
        	int i = 0;
        	Atleta atle;
        	while(i<_participantes.longitud()) {
        		if(_participantes.iesimo(i).ciaNumber() == ciaNumber) {
        			atle = _participantes.iesimo(i);
        		}
        		i++;
        	}
        	return atle;
        }
        bool mismoDoping(const Competencia& c) const {
        	int i = 0;
        	bool igual = true;
        	if(_controlAntidoping.longitud() == c.lesTocoControlAntidoping().longitud()) {
        		while(i<_controlAntidoping.longitud()) {
        			if(!c.lesTocoControlAntidoping().pertenece(atletaConCia(_controlAntidoping.iesimo(i).first)) ||
        					!(c.leDioPositivo(atletaConCia(_controlAntidoping.iesimo(i).first)) == _controlAntidoping.iesimo(i).second)) {
        				igual = false;
        			}
        			i++;
        		}
        		return igual;
        	}
        	else {
        		return false;
        	}
        }
        bool mismosParticipantes(const Competencia& c) const {
        	int i = 0;
        	bool igual = true;
        	if(_participantes.longitud() == c.participantes().longitud()) {
        		while(i<_participantes.longitud()) {
        			if(!c.participantes().pertenece(_participantes.iesimo(i))) {
        				igual = false;
        			}
        			i++;
        		}
        		return igual;
        	}
        	else {
        		return false;
        	}
        }
        enum {ENCABEZADO_ARCHIVO = 'C'};
};
std::ostream & operator<<(std::ostream & os,const Competencia & c);

#endif // COMPETENCIA_H
