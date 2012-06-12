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
	bool vale = true;
	if((_finalizada && c.finalizada()) || (!_finalizada && !c.finalizada())) {
		vale = mismosParticipantes(c) && mismoDoping(c) && ranking() == c.ranking();
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
	os << endl << "Participantes:" << endl;
	int i=0;
	while(i<participantes().longitud()) {
		os << "(";
		participantes().iesimo(i).guardar(os);
		os << ")";
		i++;
		if(i<participantes().longitud()) {
			os << ",";
		}
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
		os << "No" << endl;
	}

}


void Competencia::guardar(std::ostream& os) const {
	os << "C (|" << _categoria.first << "|, |";
	if(_categoria.second == Masculino) {
		os << "Masculino";
	}
	else {
		os << "Femenino";
	}
	os << "|) |";
	if(finalizada()) {
		os << "True";
	}
	else {
		os << "False";
	}
	os << "| [";
	int i=0;
	while(i<participantes().longitud()) {
		os << "(";
		participantes().iesimo(i).guardar(os);
		os << ")";
		i++;
		if(i<participantes().longitud()) {
			os << ",";
		}
	}
	os << "] ";
	if(finalizada()) {
		os << "[";
		i=0;
		while(i<ranking().longitud()) {
			os << ranking().iesimo(i).ciaNumber();
			i++;
			if(i<ranking().longitud()) {
				os << ",";
			}
		}
		os << "] [";
		i=0;
		while(i<lesTocoControlAntidoping().longitud()) {
			os << "(" << lesTocoControlAntidoping().iesimo(i).ciaNumber() << ", " << "|";
			if(leDioPositivo(lesTocoControlAntidoping().iesimo(i))) {
				os << "True" << "|)";
			}
			else {
				os << "False" << "|)";
			}
			i++;
			if(i<lesTocoControlAntidoping().longitud()) {
				os << ",";
			}
		}
		os << "]";
	}
	else {
		os << "[] []";
	}
}

/*
C (|Rugby|, |Masculino|) |True|
[(A |Juan| |Masculino| 1920 |Argentina| 1 [(|Football|, 35), (|Rugby|, 10)]),
(A |Jorge| |Masculino| 1930 |Argentina| 2 [(|Football|, 32), (|Rugby|, 20)]),
(A |Jackson| |Masculino| 1935 |Escocia| 6 [(|Basket|, 25), (|Football|, 40), (|Rugby|, 5)])]
[1, 6] [(1, |True|), (6, |True|)]
 */

void Competencia::cargar (std::istream& is) {
	char c;
	string stringDeporte;
	string stringSexo;
	Sexo sexo;
	string stringFinalizada;
	//Saco C , ( y |
	is >> c >> c >> c;
	getline(is, stringDeporte, '|');
	//Saco , y |
	is >> c >> c;
	getline(is, stringSexo, '|');
	if(stringSexo == "Masculino") {
		sexo = Masculino;
	}
	else {
		sexo = Femenino;
	}
	//Saco ) y |
	is >> c >> c;
	getline(is, stringFinalizada, '|');
	if(stringFinalizada == "True") {
		_finalizada = true;
	}
	else {
		_finalizada = false;
	}
	//Saco [
	is >> c;
	Lista<Atleta> atletas;
	//Me fijo si atletas no es vacio
	if(is.peek() != ']') {
		bool loop = true;
		while(loop) {
			//Saco (
			is >> c;
			Atleta atle;
			atle.cargar(is);
			atletas.agregarAtras(atle);
			//Saco )
			is >> c;
			if(is.peek() != ',') {
				loop = false;
			}
			else {
				//Saco la ,
				is >> c;
			}
		}
	}
	//Saco ]
	is >> c;
	_participantes = atletas;
	_categoria = make_pair(stringDeporte, sexo);
	if(_finalizada) {
		Lista<int> ranking;
		Lista<pair<int, bool> > doping;
		bool loop = true;
		//Saco [
		is >> c;
		//Me fijo si el ranking no esta vacio
		if(is.peek() != ']') {
			while(loop) {
				int ciaNumber;
				is >> ciaNumber;
				if(is.peek() != ',') {
					loop = false;
				}
				else {
					//Saco la ,
					is >> c;
				}
				ranking.agregarAtras(ciaNumber);
			}
		}
		//Saco ] y [
		is >> c >> c;
		//Me fijo si el doping no esta vacio
		if(is.peek() != ']') {
			loop = true;
			while(loop) {
				int ciaNumber;
				bool positive;
				string dopingStatus;
				//Saco (, ciaNumber, la , y |
				is >> c >> ciaNumber >> c >> c;
				getline(is, dopingStatus, '|');
				//Saco )
				is >> c;
				if(is.peek() != ',') {
					loop = false;
				}
				else {
					//Saco la ,
					is >> c;
				}
				if(dopingStatus == "True") {
					positive = true;
				}
				else {
					positive = false;
				}
				doping.agregarAtras(make_pair(ciaNumber, positive));
			}
		}
		finalizar(ranking, doping);
	}
	else {
		//Saco [] []
		is >> c >> c >> c >> c;
	}
}

//AUX
Atleta Competencia::atletaConCia(const int ciaNumber) const {
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
bool Competencia::mismoDoping(const Competencia& c) const {
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
bool Competencia::mismosParticipantes(const Competencia& c) const {
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

