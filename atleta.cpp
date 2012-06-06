#include <iostream>
#include "tipos.h"
#include "lista.h"
#include "atleta.h"

using namespace std;

int main() {
    cout << "Hola atleta querido" << endl;
    Atleta pepe("Pepe", Masculino, 1991, "Argentina", 123);
    pepe.entrenarNuevoDeporte("Canoa", 12);
    pepe.entrenarNuevoDeporte("Bmx", 80);
    pepe.entrenarNuevoDeporte("Natacion", 40);
    pepe.entrenarNuevoDeporte("Pedo", 60);
    pepe.entrenarNuevoDeporte("Rascarse", 50);
    cout << "Nombre: " << pepe.nombre() << " Sexo: " << pepe.sexo() << " Anio: " << pepe.anioNacimiento() <<
    		" Pais: " << pepe.nacionalidad() << " Cia number: " << pepe.nacionalidad() << endl;
    cout << "Deportes: " << pepe.deportes() << endl;
    Lista<Deporte> deporte;
    deporte.agregarAtras("caca");
    deporte.agregarAtras("caca1");
    deporte.agregarAtras("caca2");
    //deporte.agregarAtras("caca2");
    cout << "Deporte: " << deporte << endl;
    return 0;
}
