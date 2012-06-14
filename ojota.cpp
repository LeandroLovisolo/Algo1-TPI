#include <iostream>
#include "interfaz.h"

using namespace std;

Atleta atletaProdigio(const JJOO & j) {
	Atleta atletaMinimaEdad = j.competenciasFinalizadasConOroEnPodio().iesimo(0).ranking().iesimo(0);
	int i=1;
	int cantidadCompetenciasConOro = j.competenciasFinalizadasConOroEnPodio().longitud();
	while(i<cantidadCompetenciasConOro) {
		int anioAtleta = j.competenciasFinalizadasConOroEnPodio().iesimo(i).ranking().iesimo(0).anioNacimiento();
		int anioAtletaEdadMinima = atletaMinimaEdad.anioNacimiento();
		if(anioAtleta >= anioAtletaEdadMinima) {
			atletaMinimaEdad = j.competenciasFinalizadasConOroEnPodio().iesimo(i).ranking().iesimo(0);
		}
		i++;
	}
	return atletaMinimaEdad;
}

int main(){
    MenuPrincipal();
    return 0;
}

