#include <iostream>
#include "interfaz.h"

using namespace std;

Atleta atletaProdigio(const JJOO & j) {
	int i=1;
	//estado E0:
	//vale: j == pre(j) ^ i== 1
	Atleta atletaMinimaEdad = j.competenciasFinalizadasConOroEnPodio().iesimo(0).ranking().iesimo(0);
	//Vale Pc: i == 1 ^ |competenciasFinalizadasConOroEnPodio(j)| >= 1 ^ atletaMinimaEdad == competenciasFinalizadasConOroEnPodio(j)[0]
	while(i<j.competenciasFinalizadasConOroEnPodio().longitud()) {
		//invariante I: 1 <= i <= |competenciasFinalizadasConOroEnPodio(j)| ^
				//(V a <- [1...i)) anioNacimiento(atletaMinimaEdad) <= anioNacimiento(ranking(competenciasFinalizadasConOroEnPodio(j)[i])[0])
		//variante: |competenciasFinalizadasConOroEnPodio(j)| - i
		//estado Ec1:
		//vale: I ^ i < |competenciasFinalizadasConOroEnPodio(j)|
		int anioAtleta = j.competenciasFinalizadasConOroEnPodio().iesimo(i).ranking().iesimo(0).anioNacimiento();
		int anioAtletaEdadMinima = atletaMinimaEdad.anioNacimiento();
		//estado Ec2:
		//vale: anioAtleta == anioNacimiento(ranking(competenciasFinalizadasConOroEnPodio(j@E0)[i@Ec1])[0])
		//vale: anioAtletaEdadMinima == anioNacimiento(atletaMinimaEdad@Ec1)
		//vale: i == i@Ec2
		if(anioAtleta >= anioAtletaEdadMinima) {
			atletaMinimaEdad = j.competenciasFinalizadasConOroEnPodio().iesimo(i).ranking().iesimo(0);
		}
		i++;
	}
	//estado E1:
	//Vale Qc: (atletaMinimaEdad e [ranking(c)[0] | c <- competenciasFinalizadasConOroEnPodio(j)] ) ^
	//(V a <- competenciasFinalizadasConOroEnPodio(j),a != atletaMinimaEdad) anioNacimiento(ranking(atletaMinimaEdad)[0]) >= anioNacimiento(ranking(a)[0])

	return atletaMinimaEdad;
}

int main(){
    MenuPrincipal();
    return 0;
}

