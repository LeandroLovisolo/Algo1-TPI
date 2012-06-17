#include <iostream>
#include "interfaz.h"
#include "lista.h"

using namespace std;

// problema obtenerCampeones(j: JJOO) = result: [Atleta] {
//     asegura result == [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0];
// }

Lista<Atleta> obtenerCampeones(const JJOO & j) {
	Lista<Competencia> competenciasOro = j.competenciasFinalizadasConOroEnPodio();
	Lista<Atleta> campeones;
	int i = 0;

	// Vale Pc: competenciasOro == [c | c <- competencias(j), finalizada(c) && |ranking(c)| > 0] &&
	//          |campeones| == 0 &&
	//          i == 0
	// Implica  0 <= i <= |competenciasOro|
	// Implica  campeones == [] == [cab(ranking(c)) | c <- competenciasOro[0..i)]
	//
	// Luego, Pc -> I.
	while(i < competenciasOro.longitud()) {
		// Guarda B:     i < |competenciasOro|
		// Invariante I: 0 <= i <= |competenciasOro| && campeones = [cab(ranking(c)) | c <- competenciasOro[0..i)]
		// Variante V:   |competenciasOro| - i
		// Cota C:       0

		// Estado E1
		// Vale I && B
		campeones.agregarAtras(competenciasOro.iesimo(i).ranking().cabeza());

		// Estado E2
		// Vale    campeones == campeones@E1 ++ [cab(ranking(competenciasOro[i]))]
		// Implica campeones == [cab(ranking(c)) | c <- competenciasOro[0..i)] ++ [cab(ranking(competenciasOro[i]))]
		// Implica campeones == [cab(ranking(c)) | c <- competenciasOro[0..i]]
		i++;

		// Estado E3
		// Vale    i == i@E2 + 1 && campeones == campeones@E2
		// Implica 0 < i
		// Implica i == i@E2 + 1 < |competenciasOro| + 1
		// Implica i == i@E2 + 1 <= |competenciasOro|
		// Implica i <= |competenciasOro|
		// Implica 0 <= i <= |competenciasOro|
		// Implica campeones == [cab(ranking(c)) | c <- competenciasOro[0..i@E2]]
		// Implica i@E2 == i - 1
		// Implica campeones == [cab(ranking(c)) | c <- competenciasOro[0..i - 1]]
		// Implica campeones == [cab(ranking(c)) | c <- competenciasOro[0..i)]
		// Implica 0 <= i <= |competenciasOro| && campeones == [cab(ranking(c)) | c <- competenciasOro[0..i)]
		//
		// Luego, E3 -> I.
		//
		// Vale    V == |competenciasOro| - i@E3
		// Implica V == |competenciasOro| - (i@E1 + 1)
		// Implica V == |competenciasOro| - i@E1 - 1 < |competenciasOro| - i@E1 == V@E1
		//
		// Luego, V@E3 < V@E1.
		//
		// Supongo V <= C.
		// Vale    I && (V <= C)
		// Implica 0 <= i <= |competenciasOro| && campeones = [cab(ranking(c)) | c <- competenciasOro[0..i)] && (|competenciasOro| - i <= 0)
		// Implica |competenciasOro| <= i
		// Implica i == |competenciasOro|
		// Implica ¬(i < |competenciasOro|)
		//
		// Luego, I && (V <= C) -> ¬B.
	}

	// Vale Qc: i == |competenciasOro| && campeones = [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0]
	//
	// Vale     I && ¬B
	// Implica  0 <= i <= |competenciasOro| && campeones = [cab(ranking(c)) | c <- competenciasOro[0..i)] && i >= |competenciasOro|
	// Implica  i <= |competenciasOro| && i >= |competenciasOro|
	// Implica  i == |competenciasOro|
	// Implica  campeones = [cab(ranking(c)) | c <- competenciasOro[0..|competenciasOro|)]
	// Implica  campeones = [cab(ranking(c)) | c <- competenciasOro]
	// Implica  campeones = [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0]
	// Implica  i == |competenciasOro| && campeones = [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0]
	//
	// Luego, (I && ¬B) -> Qc.

	return campeones;

	// Vale    result == campeones@Qc
	// Implica result == [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0]
}


// problema atletaProdigio (j: JJOO) = result: Atleta {
//     requiere algunaVezSeCompitio: |competenciasConOroEnPodio(j)| > 0;
//     asegura esCampeon(result, j);
//     asegura (∀c ∈ competenciasConOroEnPodio(j)) añoNacimiento(campeon(c)) ≤ añoNacimiento(result);
//
//     aux esCampeon(a: Atleta, j: JJOO): Bool =
//         (∃c ∈ competenciasConOroEnPodio(j)) a == campeon(c);
//     aux campeon (c: Competencia): Atleta = cab(ranking(c));
//     aux competenciasConOroEnPodio(j: JJOO): [Competencia] =
//         [c|c ← competencias(j), finalizada(c) ∧ |ranking(c)| > 0];
// }

Atleta atletaProdigio(const JJOO & j) {
	Lista<Atleta> campeones = obtenerCampeones(j);
	Atleta prodigio = campeones.cabeza();
	int i = 1;

	// Vale Pc: campeones == [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0] &&
	//          |campeones| > 0 &&
	//          prodigio == cab(campeones) &&
	//          i == 1
	// Implica  prodigio ∈ campeones
	// Implica  1 <= i <= |campeones|
	// Implica  (∀c ∈ campeones[0..1)) añoNacimiento(prodigio) >= añoNacimiento(c)
	// Implica  (∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)
	// Implica  1 <= i <= |campeones| && prodigio ∈ campeones && (∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)
	//
	// Luego, Pc -> I.
	while(i < campeones.longitud()) {
		// Guarda B:     i < |campeones|
		// Invariante I: 1 <= i <= |campeones| && prodigio ∈ campeones && (∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)
		// Variante V:   |campeones| - i
		// Cota C:       0

		// Estado E1
		// Vale I && B

		if(campeones.iesimo(i).anioNacimiento() > prodigio.anioNacimiento()) {
			// Estado F
			// Vale añoNacimiento(campeones[i]) > añoNacimiento(prodigio@E1)
			prodigio = campeones.iesimo(i);

			// Estado G
			// Vale añoNacimiento(campeones[i]) > añoNacimiento(prodigio@E1) && prodigio == campeones[i]
		} else {

			// Estado H
			// Vale añoNacimiento(campeones[i]) <= añoNacimiento(prodigio)  && prodigio == prodigio@E1
		}

		// Estado E2
		// Vale    G || H
		// Implica  (añoNacimiento(campeones[i]) >  añoNacimiento(prodigio@E1) && prodigio == campeones[i]) ||
		//          (añoNacimiento(campeones[i]) <= añoNacimiento(prodigio)    && prodigio == prodigio@E1)
		// Implica   prodigio ∈ campeones
		// Implica   añoNacimiento(prodigio) >  añoNacimiento(prodigio@E1) ||
		//           añoNacimiento(prodigio) == añoNacimiento(prodigio@E1)
		// Implica ((∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >  añoNacimiento(prodigio@E1) >= añoNacimiento(c)) ||
		//         ((∀c ∈ campeones[0..i)) añoNacimiento(prodigio) == añoNacimiento(prodigio@E1) >= añoNacimiento(c))
		// Implica   prodigio ∈ campeones && (∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)

		i++;
		// Estado E3
		// Vale    i == i@E2 + 1 && prodigio ∈ campeones && (∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)
		// Implica 1 < i
		// Implica i == i@E2 + 1 < |campeones| + 1 <= |campeones|
		// Implica 1 <= i <= |campeones|
		// Implica 1 <= i <= |campeones| && prodigio ∈ campeones && (∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)
		//
		// Luego, E3 -> I.
		//
		// Vale    V = |campeones| - i@E3
		// Implica V = |campeones| - (i@E1 + 1)
		// Implica V = |campeones| - i@E1 - 1 < |campeones| - i@E1 == V@E1
		//
		// Luego, V@E3 < V@E1.
		//
		// Supongo V <= C.
		// Vale    I && (V <= C)
		// Implica 1 <= i <= |campeones| && prodigio ∈ campeones && ((∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)) && (|campeones| - i <= 0)
		// Implica |campeones| <= i
		// Implica i == |campeones|
		// Implica ¬(i < |campeones|)
		//
		// Luego, I && (V <= C) -> ¬B.
	}

	// Vale Qc: i == |campeones| && prodigio ∈ campeones && (∀c ∈ campeones) añoNacimiento(prodigio) >= añoNacimiento(c)
	//
	// Vale     I && ¬B
	// Implica  1 <= i <= |campeones| && prodigio ∈ campeones && ((∀c ∈ campeones[0..i)) añoNacimiento(prodigio) >= añoNacimiento(c)) && ¬(i < |campeones|)
	// Implica  i >= |campeones|
	// Implica  i == |campeones|
	// Implica  (∀c ∈ campeones[0..|campeones|)) añoNacimiento(prodigio) >= añoNacimiento(c)
	// Implica  (∀c ∈ campeones) añoNacimiento(prodigio) >= añoNacimiento(c)
	// Implica  i == |campeones| && prodigio ∈ campeones && (∀c ∈ campeones) añoNacimiento(prodigio) >= añoNacimiento(c)
	//
	// Luego, (I && ¬B) -> Qc.

	return prodigio;

	// Vale      result == prodigio
	// Implica   result ∈ campeones && (∀c ∈ campeones) añoNacimiento(result) >= añoNacimiento(c)
	// Implica   result ∈ [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0] &&
	//          (∀c ∈ [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0]) añoNacimiento(result) >= añoNacimiento(c)
	// Implica ((∃c ∈ [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0]) result == cab(ranking(c))) &&
	//         ((∀c ∈ [cab(ranking(c)) | c <- competencias(j), finalizada(c) && |ranking(c)| > 0]) añoNacimiento(result) >= añoNacimiento(c))
}

int main(){
    MenuPrincipal();
    return 0;
}
