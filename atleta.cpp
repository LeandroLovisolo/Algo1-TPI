#include <iostream>
#include "tipos.h"
#include "lista.h"
#include "atleta.h"

using namespace std;
Atleta::Atleta() {}
Atleta::Atleta(const string nombre,const Sexo sexo, const int anio, const Pais pais, const int ciaNumber) {
	_nombre = nombre;
    _sexo = sexo;
    _anioNacimiento = anio;
    _nacionalidad = pais;
    _ciaNumber = ciaNumber;
}

    //string nombre() const;
string Atleta::nombre() const {
        return _nombre;
}

    //Sexo sexo() const;
Sexo Atleta::sexo() const {
	return _sexo;
}

    //int anioNacimiento() const;
int Atleta::anioNacimiento() const {
	return _anioNacimiento;
}

    //Pais nacionalidad() const;
Pais Atleta::nacionalidad() const {
	return _nacionalidad;
}

    //int ciaNumber() const;
int Atleta::ciaNumber() const {
	return _ciaNumber;
}

    //Lista<Deporte> deportes() const;
Lista<Deporte> Atleta::deportes() const {
	int i=0;
    Lista<Deporte> deportes;
    while(i<_deportes.longitud()) {
        deportes.agregarAtras(_deportes.iesimo(i).first);
        i++;
    }
    return deportes;
}
    //int capacidad(const Deporte d) const;
int Atleta::capacidad(const Deporte d) const {
    int i=0;
    int capacidad;
    while(i<_deportes.longitud()) {
        if(_deportes.iesimo(i).first == d) {
            capacidad = _deportes.iesimo(i).second;
         }
         i++;
    }
    return capacidad;
}

    //void entrenarNuevoDeporte(const Deporte deporte, const int capacidad);
void Atleta::entrenarNuevoDeporte(const Deporte deporte, const int capacidad) {
	int i=0;
	Lista<pair<Deporte, int> > tempDeportes;
    pair <Deporte,int> nuevoDep = make_pair (deporte,capacidad);
    	//cout << "Tengo lista: " << this->deportes() << " Voy a agregar: "<< deporte << endl;
    if(_deportes.longitud() > 0) {
    	while(i<_deportes.longitud()) {
    			//cout << "Cliclo letra -> " << _deportes.iesimo(i).first << endl;
    		if(_deportes.iesimo(i).first < deporte && ((i+1) < _deportes.longitud()) && (_deportes.iesimo(i+1).first > deporte)) {
    			tempDeportes.agregarAtras(_deportes.iesimo(i));
    			tempDeportes.agregarAtras(nuevoDep);
    		}
    		else if(_deportes.iesimo(i).first < deporte && ((i+1) >= _deportes.longitud())) {
    				//cout << "Es "<<_deportes.iesimo(i).first<<" < "<<deporte << " y es la ultima" <<endl;
    				//cout << "-- " << tempDeportes << endl;
    			tempDeportes.agregarAtras(_deportes.iesimo(i));
    			tempDeportes.agregarAtras(nuevoDep);
    				//cout << "-- " << tempDeportes << endl;
    		}
    		else if(_deportes.iesimo(i).first == deporte) {
    			tempDeportes.agregarAtras(nuevoDep);
    		}
    		else {
    			tempDeportes.agregarAtras(_deportes.iesimo(i));
    		}
    		i++;
    	}
    }
    else {
    	pair <Deporte,int> nuevoDep = make_pair (deporte,capacidad);
    	tempDeportes.agregar(nuevoDep);
    }
    _deportes = tempDeportes;
    	//cout << "Final: " << deportes() << endl;

}

bool Atleta::operator==(const Atleta& a) const {
    bool igual = this->_nombre == a.nombre() && this->_sexo == a.sexo() && this->_anioNacimiento == a.anioNacimiento() &&
    		this->_nacionalidad == a.nacionalidad() && this->_ciaNumber == a.ciaNumber();
    int i=0;
    if(a.deportes().longitud() == this->deportes().longitud()) {
    	while(i<a.deportes().longitud()) {
    		if(!this->deportes().pertenece((a.deportes().iesimo(i))) || !(this->capacidad(a.deportes().iesimo(i)) ==
    				a.capacidad(a.deportes().iesimo(i)))) {
    			igual = false;
    		}
    	}
    	return igual;
    }
    else {
    	return false;
    }
}

void Atleta::mostrar(std::ostream& os) const {
	cout << "---- Atleta ----" << endl;
	cout << "Nombre: " << nombre() << " Sexo: ";
	if(sexo() == Masculino) {
		cout << "Masculino";
	}
	else {
		cout << "Femenino";
	}
	cout << " Anio: " << anioNacimiento() << " Pais: " << nacionalidad() << " CiaNumber: " << ciaNumber() << endl;
	cout << "Deportes:" << endl;
	cout << deportes() << endl;
}

void Atleta::guardar(std::ostream& os) const {
	//Hay que guardar A |Liu Song| |Masculino| 1972 |China| 123 [(|Tenis de Mesa|, 90)]
	os << "A |" << nombre() <<"| |";
	if(sexo() == Masculino) {
		os << "Masculino";
	}
	else {
		os << "Femenino";
	}
	os << "| " << anioNacimiento() << " |" << nacionalidad() << "| " << ciaNumber();
	os << "[";
	int i=0;
	while(i<deportes().longitud()) {
		os << "(|" << deportes().iesimo(i) << "|, " << capacidad(deportes().iesimo(i)) << ")";
		if((i+1)<deportes().longitud()) {
			os << ",";
		}
	}
	os << "]";
}

void Atleta::cargar (std::istream& is) {
	char c;
	int anio, ciaNumber, capacidad;
	string deporte, nombre, sexo, nacionalidad;
	Sexo sex;
	is >> c;
	is >> c;
	getline(is, nombre, '|');
	is >> c;
	is(is, sexo, '|');
	if(sexo == "Masculino") {
		sex = Masculino;
	}
	else {
		sex = Femenino;
	}
	is >> anio;
	is >> c;
	getline(is, nacionalidad, '|');
	is >> ciaNumber;
	//Empiezo con la lista de deportes, agarro [
	is >> c;
	bool looper = true;
	while(looper) {
		//Agarro (
		is >> c;
		//Agarro |
		is >> c;
		//Agarro el deporte
		getline(is, deporte, '|');
		//Agarro la ,
		is >> c;
		//Agarro capacidad
		is >> capacidad;
		//Agarro )
		is >> c;
		//Peek se fija sin agarrar el caracter, cual es el siguiente
		if(is.peek() != ',') {
			looper = false;
		}
		else {
			//Saco la , que delimita otro deporte, ej, [(|Tenis de Mesa|, 90),(|Bmx|, 90)]
			is >> c;
		}
	}
}