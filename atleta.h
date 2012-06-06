#ifndef ATLETA_H
#define ATLETA_H
#include <iostream>
#include "tipos.h"
#include "lista.h"

class Atleta{
    public:

        Atleta();
        Atleta(const string nombre,const Sexo sexo, const int anio, const Pais pais, const int ciaNumber) {
            _nombre = nombre;
            _sexo = sexo;
            _anioNacimiento = anio;
            _nacionalidad = pais;
            _ciaNumber = ciaNumber;
        }

        //string nombre() const;
        string nombre() const {
            return _nombre;
        }

        //Sexo sexo() const;
        Sexo sexo() const {
            return _sexo;
        }

        //int anioNacimiento() const;
        int anioNacimiento() const {
            return _anioNacimiento;
        }

        //Pais nacionalidad() const;
        Pais nacionalidad() const {
            return _nacionalidad;
        }

        //int ciaNumber() const;
        int ciaNumber() const {
            return _ciaNumber;
        }

        //Lista<Deporte> deportes() const;
        Lista<Deporte> deportes() const {
            int i=0;
            Lista<Deporte> deportes;
            while(i<_deportes.longitud()) {
                deportes.agregar(_deportes.iesimo(i).first);
                i++;
            }
            return deportes;
        }
        //int capacidad(const Deporte d) const;
        int capacidad(const Deporte d) const {
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
        void entrenarNuevoDeporte(const Deporte deporte, const int capacidad) {
        	int i=0;
        	Lista<pair<Deporte, int> > tempDeportes;
        	pair <Deporte,int> nuevoDep = make_pair (deporte,capacidad);
        	cout << "Tengo lista: " << this->deportes() << "Voy a agregar: "<< deporte << endl;
        	if(_deportes.longitud() > 0) {
        		while(i<_deportes.longitud()) {
        			cout << "Ciclo="<<i<< endl;
					if(_deportes.iesimo(i).first < deporte) {
						if((((i+1) < _deportes.longitud()) && (_deportes.iesimo(i+1).first > deporte)) || ((i+1) >= _deportes.longitud())) {
							tempDeportes.agregarAtras(_deportes.iesimo(i));
							tempDeportes.agregarAtras(nuevoDep);
						}
						else {
							tempDeportes.agregarAtras(_deportes.iesimo(i));
						}
					}
					else if(_deportes.iesimo(i).first == deporte) {
						tempDeportes.agregarAtras(nuevoDep);
					}
					else if(i == 0) {
						cout << "Es mas chico "<< endl;
						tempDeportes.agregarAtras(nuevoDep);
						tempDeportes.agregarAtras(_deportes.iesimo(i));
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

        }

        bool operator==(const Atleta& a) const;

        //void mostrar(std::ostream& os) const;
		//void guardar(std::ostream& os) const;
		//void cargar (std::istream& is);

    private:
        string _nombre;
        Sexo _sexo;
        int _anioNacimiento;
        Pais _nacionalidad;
        int _ciaNumber;
        Lista<pair<Deporte, int> > _deportes;

        enum {ENCABEZADO_ARCHIVO = 'A'};
};

std::ostream & operator<<(std::ostream & os,const Atleta & a);

#endif // ATLETA_H
