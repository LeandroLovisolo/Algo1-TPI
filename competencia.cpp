#include <iostream>
#include "competencia.h"
#include "lista.h"
#include "tipos.h"

Competencia::Competencia() {}
Competencia::Competencia(const Deporte d, const Sexo s, const Lista<Atleta>& participantes) {
	_categoria = make_pair(d, s);
	_participantes = participantes;
	_finalizada = false;
}
Categoria Competencia::categoria() const {
	return _categoria;
}

Lista<Atleta> Competencia::participantes() const {
	return _participantes;
}

bool Competencia::finalizada() const {
	return _finalizada;
}

Lista<Atleta> Competencia::ranking() const {
	int i=0;
    Lista<Atleta> tempAtletas;
    while(i<_ranking.longitud()) {
    	tempAtletas.agregarAtras(this->atletaConCia(_ranking.iesimo(i)));
        i++;
    }
    return tempAtletas;
}

Lista<Atleta> Competencia::lesTocoControlAntidoping() const {
	Lista<Atleta> tempAtle;
    int i = 0;
    while(i<_controlAntidoping.longitud()) {
    	tempAtle.agregarAtras(atletaConCia(_controlAntidoping.iesimo(i).first));
        i++;
    }
    return tempAtle;
}

bool Competencia::leDioPositivo(const Atleta& a) const {
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

void Competencia::finalizar(const Lista<int>& posiciones, const Lista<pair<int, bool> >& control) {
	_finalizada = true;
    _ranking = posiciones;
    _controlAntidoping = control;
}

void Competencia::linfordChristie(const int ciaNum) {
	Atleta atle = atletaConCia(ciaNum);
	_participantes.sacar(atle);
}

bool Competencia::gananLosMasCapaces() const {
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

void Competencia::sancionarTramposos() {
	int i=0;
	while(i<_controlAntidoping.longitud()) {
        if(_controlAntidoping.iesimo(i).second) {
        	_ranking.sacar(_controlAntidoping.iesimo(i).first);
        }
        i++;
    }
}

bool Competencia::operator==(const Competencia& c) const {
	bool vale;
	if((_finalizada && c.finalizada()) || (!_finalizada && !c.finalizada())) {
		vale = mismosParticipantes(c) && mismoDoping(c);
	}
	else {
		vale = false;
	}
	return vale;
}

void Competencia::mostrar(std::ostream& os) const {
	os << "Competencia:" << endl << "Categoria: " << _categoria.first << " ";
	if(_categoria.second == Masculino) {
		os << "Masculino";
	}
	else {
		os << "Femeninio";
	}
	os << endl << "Finalizada: ";
	if(_finalizada) {
		os << "Si" << endl;
		int i = 0;
		os << "Ranking: [";
		while(i<ranking().longitud()) {
			os << ranking().iesimo(i).nombre();
			i++;
			if(i<ranking().longitud()) {
				os << ",";
			}
		}
		os << "]" << endl;
		i=0;
		os << "Dopping : [";
		while(i<lesTocoControlAntidoping().longitud()) {
			os << "<" << lesTocoControlAntidoping().iesimo(i).nombre() << ",";
			os << leDioPositivo(lesTocoControlAntidoping().iesimo(i)) << ">";
			i++;
			if(i<lesTocoControlAntidoping().longitud()) {
				os << ",";
			}
		}
		os << "]" << endl;
	}
	else {
		os << "No";
	}

}

void Competencia::guardar(std::ostream& os) const {

}

void Competencia::cargar (std::istream& is) {

}
