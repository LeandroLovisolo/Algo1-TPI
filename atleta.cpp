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
    Lista<pair<Deporte, int> > newDeportes;
    bool agregado = false;
    int i = 0;
    while(i < _deportes.longitud()) {
        if(deporte < _deportes.iesimo(i).first && !agregado) {
            newDeportes.agregarAtras(make_pair(deporte, capacidad));
            newDeportes.agregarAtras(_deportes.iesimo(i));
            agregado = true;
        } else if(deporte == _deportes.iesimo(i).first) {
            newDeportes.agregarAtras(make_pair(deporte, capacidad));
            agregado = true;
        } else {
            newDeportes.agregarAtras(_deportes.iesimo(i));
        }
        i++;
    }
    if(!agregado) {
        newDeportes.agregarAtras(make_pair(deporte, capacidad));
    }
    _deportes = newDeportes;
}

bool Atleta::operator==(const Atleta& a) const {
    bool igual = _nombre         == a._nombre         &&
    		     _sexo           == a._sexo           &&
    		     _anioNacimiento == a._anioNacimiento &&
    		     _nacionalidad   == a._nacionalidad   &&
    		     _ciaNumber      == a._ciaNumber;

    int i=0;
    if(a.deportes() == this->deportes()) {
        while(i<a.deportes().longitud()) {
            if(!(this->capacidad(a.deportes().iesimo(i)) == a.capacidad(a.deportes().iesimo(i)))) {
                igual = false;
            }
            i++;
        }
    }
    else {
        igual = false;
    }
    return igual;
}

void Atleta::mostrar(std::ostream& os) const {
    string sexo = "Masculino";
    if(_sexo == Femenino) sexo = "Femenino";

    os << _nombre << " (#" << _ciaNumber << "). " <<
          sexo << ". Año nac.: " << _anioNacimiento << ". " <<
          "Nacionalidad: " << _nacionalidad << ". " << "Deportes: ";

    if(_deportes.longitud() == 0) {
        os << "ninguno.";
    } else {
        int i = 0;
        while(i < _deportes.longitud()) {
            os << endl << "  " << _deportes.iesimo(i).first << " (" <<
                                  _deportes.iesimo(i).second << ")";
            i++;
        }
    }

    os << endl;
}

void Atleta::guardar(std::ostream& os) const {
    //Hay que guardar A |Liu Song| |Masculino| 1972 |China| 123 [(|Tenis de Mesa|, 90)]
    // A |Pepe| |Masculino| 1991 |Arg| 1 [(|Football|, 13),(|Karate|, 44),(|Zunga|, 17)]
    os << "A |" << nombre() <<"| |";
    if(sexo() == Masculino) {
        os << "Masculino";
    }
    else {
        os << "Femenino";
    }
    os << "| " << anioNacimiento() << " |" << nacionalidad() << "| " << ciaNumber();
    os << " [";
    int i=0;
    while(i<deportes().longitud()) {
        os << "(|" << deportes().iesimo(i) << "|, " << capacidad(deportes().iesimo(i)) << ")";
        if((i+1)<deportes().longitud()) {
            os << ",";
        }
        i++;
    }
    os << "]";
}

void Atleta::cargar (std::istream& is) {
    //A |Pepe| |Masculino| 1991 |Arg| 1 [(|Football|, 13),(|Karate|, 44),(|Zunga|, 17)]
    char c;
    int capacidad;
    string deporte, sexo;
    is >> c;
    is >> c;
    getline(is, _nombre, '|');
    is >> c;
    getline(is, sexo, '|');
    if(sexo == "Masculino") {
        _sexo = Masculino;
    }
    else {
        _sexo = Femenino;
    }
    is >> _anioNacimiento;
    is >> c;
    getline(is, _nacionalidad, '|');
    is >> _ciaNumber;
    //Empiezo con la lista de deportes, agarro [
    is >> c;
    Lista<pair<Deporte, int> > tempDeportes;
    if(is.peek() != ']') {
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
            entrenarNuevoDeporte(deporte, capacidad);
        }
    }
    //Saco el ultimo ]
    is >> c;
}
