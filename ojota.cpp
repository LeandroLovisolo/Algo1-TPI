#include <iostream>
#include "interfaz.h"

using namespace std;

Atleta atletaProdigio(const JJOO & j) {
	int i=1;
	//estado E0:
	//vale: j == pre(j) ^ i== 1
	Atleta atletaMinimaEdad = j.competenciasFinalizadasConOroEnPodio().iesimo(0).ranking().iesimo(0);
	//Vale Pc: j == j@E0 ^ i == 1 ^ |competenciasFinalizadasConOroEnPodio(j)| >= 1 ^ atletaMinimaEdad == competenciasFinalizadasConOroEnPodio(j)[0]
	while(i<j.competenciasFinalizadasConOroEnPodio().longitud()) {
		//invariante I: 1 <= i <= |competenciasFinalizadasConOroEnPodio(j)| ^
				//(V a <- [1...i)) anioNacimiento(atletaMinimaEdad) <= anioNacimiento(ranking(competenciasFinalizadasConOroEnPodio(j)[i])[0]) ^ j@Pc
		//variante: |competenciasFinalizadasConOroEnPodio(j)| - i
		//Guarda B: i < |competenciasFinalizadasConOroEnPodio(j)|

		//estado Pif:
		//vale: I ^ B
		if(j.competenciasFinalizadasConOroEnPodio().iesimo(i).ranking().iesimo(0).anioNacimiento() >=
							atletaMinimaEdad.anioNacimiento()) {
			//Eif1:
			//vale i == i@Pif ^ j@Pif ^ anioNacimiento(ranking(competenciasConOroEnPodio(j)[i])[0]) >= anioNacimiento(atletaMinimaEdad@Pif)
			atletaMinimaEdad = j.competenciasFinalizadasConOroEnPodio().iesimo(i).ranking().iesimo(0);
			//Eif2:
			//vale: i == i@Eif1 ^ j == j@Eif1 ^ atletaMinimaEdad == ranking(competenciasConOroEnPodio(j)[i])[0]
		}
		//Qif:
		//vale: i == i@Pif ^ j@Pif
		//vale: atletaMinimaEdad == deberia ser el max entre la edad de atletaMinimaEdad@Pif y ranking(competenciasConOroEnPodio(j)[i])[0]
		i++;
		//Ec1:
		//vale: atletaMinimaEdad == lo de Qif ^ j == j@Qif ^ i == i@Qif +1
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

