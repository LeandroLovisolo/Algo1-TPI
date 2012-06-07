#ifndef COMPETENCIA_H
#define COMPETENCIA_H

#include "atleta.h"
#include "lista.h"

class Competencia{
    public:

        Competencia() {}
        //Competencia(const Deporte d, const Sexo s, const Lista<Atleta>& participantes);
        Competencia(const Deporte d, const Sexo s, const Lista<Atleta>& participantes) {
        	_categoria = make_pair(d, s);
        	_participantes = participantes;
        	_finalizada = false;
        }
        //Categoria categoria() const;
        Categoria categoria() const {
        	return _categoria;
        }

        //Lista<Atleta> participantes() const;
        Lista<Atleta> participantes() const {
        	return _participantes;
        }

        //bool finalizada() const;
        bool finalizada() const {
        	return _finalizada;
        }

        //Lista<Atleta> ranking() const;
        Lista<Atleta> ranking() const {
        	int i=0;
        	Lista<Atleta> tempAtletas;
        	while(i<_ranking.longitud()) {
        		tempAtletas.agregarAtras(this->atletaConCia(_ranking.iesimo(i)));
        		i++;
        	}
        	return tempAtletas;
        }

        //Lista<Atleta> lesTocoControlAntidoping() const;
        Lista<Atleta> lesTocoControlAntidoping() const {
        	Lista<Atleta> tempAtle;
        	int i = 0;
        	while(i<_controlAntidoping.longitud()) {
        		tempAtle.agregarAtras(atletaConCia(_controlAntidoping.iesimo(i).first));
        		i++;
        	}
        	return tempAtle;
        }

        //bool leDioPositivo(const Atleta& a) const;
        bool leDioPositivo(const Atleta& a) const {
           	int i = 0;
           	bool leDio = false;
            while(i<_controlAntidoping.longitud()) {
            	if(_controlAntidoping.iesimo(i).first == a.ciaNumber() && _controlAntidoping.iesimo(i).second) {
            		leDio = true;
            	}
            	i++;
            }
            return leDio;
        }

        //void finalizar(const Lista<int>& posiciones, const Lista<pair<int, bool> >& control);
        void finalizar(const Lista<int>& posiciones, const Lista<pair<int, bool> >& control) {
        	_finalizada = true;
        	_ranking = posiciones;
        	_controlAntidoping = control;

        }

        void linfordChristie(const int ciaNum) {
        	Atleta atle = atletaConCia(ciaNum);
        	_participantes.sacar(atle);
        }
        //bool gananLosMasCapaces() const;
        bool gananLosMasCapaces() const {
        	int i=0;
        	bool masCapaces = true;
        	while(i<_ranking.longitud()) {
        		if((i+1) < _ranking.longitud() && !(atletaConCia(_ranking.iesimo(i)).capacidad(categoria().first) >
        			atletaConCia(_ranking.iesimo(i+1)).capacidad(categoria().first))) {
        				masCapaces = false;
        		}
        		i++;
        	}
        	return masCapaces;
        }
        //void sancionarTramposos();
        void sancionarTramposos() {
        	int i=0;
        	while(i<_controlAntidoping.longitud()) {
        		if(_controlAntidoping.iesimo(i).second) {
        			_ranking.sacar(_controlAntidoping.iesimo(i).first);
        		}
        		i++;
        	}
        }

        //bool operator==(const Competencia& c) const;
        bool operator==(const Competencia& c) const {
        	if((_finalizada && c.finalizada()) || (!_finalizada && !c.finalizada())) {
        		return mismosParticipantes(c) && mismoDoping(c);
        	}
        	else {
        		return false;
        	}
        }
        //void mostrar(std::ostream& os) const;
        void mostrar(std::ostream& os) const {

        }
        //void guardar(std::ostream& os) const;
        void guardar(std::ostream& os) const {

        }
		//void cargar (std::istream& is);
        void cargar (std::istream& is) {

        }
	private:

        Categoria _categoria;
        Lista<Atleta> _participantes;
        bool _finalizada;
        Lista<int> _ranking;
        Lista<pair<int, bool> > _controlAntidoping;
        Atleta atletaConCia(const int ciaNumber) const {
        	int i = 0;
        	Atleta atle;
            /*string nombre;
            Sexo sexo;
            int anioNacimiento;
            Pais nacionalidad;
            int ciaNumb;
            Lista<pair<Deporte, int> > _deportes;
            */
        	while(i<_participantes.longitud()) {
        		if(_participantes.iesimo(i).ciaNumber() == ciaNumber) {
        			atle = _participantes.iesimo(i);
        			//ciaNumb = _participantes.iesimo(i).ciaNumber();
        			//sexo = _participantes.iesimo(i).sexo();
        			//nombre = _participantes.iesimo(i).nombre();
        			//anioNacimiento = _participantes.iesimo(i).anioNacimiento();
        			//nacionalidad = _participantes.iesimo(i).nacionalidad();
        		}
        		i++;
        	}
        	//Atleta atle(nombre, sexo, anioNacimiento, nacionalidad, ciaNumb);
        	/*i=0;
        	while(i<atle.deportes().longitud()) {
        		atle.entrenarNuevoDeporte(atle.deportes().iesimo(i), atle.capacidad(atle.deportes().iesimo(i)));
        		i++;
        	}*/
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
