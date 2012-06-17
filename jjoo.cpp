#include <iostream>
#include "tipos.h"
#include "lista.h"
#include "jjoo.h"

JJOO::JJOO () {
	_jornadaActual = 1;
	_competenciasPorDia.agregar(Lista<Competencia>());
}

JJOO::JJOO (const int anio, const Lista<Atleta>& atletas, const Lista<Lista<Competencia> >& competenciasPorDia){
    _anio = anio;
    _atletas = atletas;
    _jornadaActual = 1;
    _competenciasPorDia = competenciasPorDia;
}

int JJOO::anio() const{
    return _anio;
}

Lista<Atleta> JJOO::atletas() const {
    return _atletas;
}

int JJOO::cantDias() const {
    return _competenciasPorDia.longitud();
}

int JJOO::jornadaActual() const {
    return _jornadaActual;
}

Lista<Competencia> JJOO::cronograma(const int dia) const {
    return _competenciasPorDia.iesimo(dia - 1);
}

Lista<Competencia> JJOO::competencias() const {
    Lista<Competencia> competencias;
    int i = 0;
	while (i < cantDias()) {
		competencias.concatenar(_competenciasPorDia.iesimo(i));
		i++;
	}
    return competencias;
}

Lista<Competencia> JJOO::competenciasFinalizadasConOroEnPodio() const {
	Lista<Competencia> result;
	int i = 0;
	while (i < competencias().longitud())   {
		Competencia actual = competencias().iesimo(i);
		if (actual.finalizada() && actual.ranking().longitud() > 0) {
			result.agregar(actual);
		}
		i++;
	}
	return result;
}

Lista<Atleta> JJOO::dePaseo() const {
    Lista<Atleta> result;
    int i = 0;
	while (i < _atletas.longitud()) {
		if (!participantes().pertenece(_atletas.iesimo(i))) {
			result.agregar(_atletas.iesimo(i));
		}
		i++;
	}
	return result;
}

Lista<pair<Pais, Lista<int> > > JJOO::medallero() const {
	Lista<Pais> paises = this->paises();
	Lista<pair<Pais, Lista<int> > > medallero;

	// Recorro la lista de países
	int i = 0;
	while(i < paises.longitud()) {
		Pais pais = paises.iesimo(i);
		int oros = 0;
		int platas = 0;
		int bronces = 0;

		// Recorro los rankings de todas las competencias y acumulo las medallas del país actual
		int j = 0;
		while(j < competencias().longitud()) {
			Lista<Atleta> ranking = competencias().iesimo(j).ranking();
			if(ranking.longitud() > 0 && ranking.iesimo(0).nacionalidad() == pais) oros++;
			if(ranking.longitud() > 1 && ranking.iesimo(1).nacionalidad() == pais) platas++;
			if(ranking.longitud() > 2 && ranking.iesimo(2).nacionalidad() == pais) bronces++;
			j++;
		}

		// Agrego al medallero la tupla país/medallas
		Lista<int> medallas;
		medallas.agregarAtras(oros);
		medallas.agregarAtras(platas);
		medallas.agregarAtras(bronces);
		medallero.agregar(make_pair(pais, medallas));

		i++;
	}

	// Devuelvo el medallero ordenado
	return ordenarMedallero(medallero);
}

Lista<Pais> JJOO::paises() const {
	Lista<Pais> paises;
	int i = 0;
	while(i < participantes().longitud()) {
		Pais actual = participantes().iesimo(i).nacionalidad();
		if(!paises.pertenece(actual)) {
			paises.agregar(actual);
		}
		i++;
	}
	return paises;
}

Lista<pair<Pais,Lista<int> > > JJOO::ordenarMedallero(const Lista<pair<Pais,Lista<int> > > & medallero) const {

	// Guardo acá mi copia ordenada del medallero.
	Lista<pair<Pais, Lista<int> > > ordenado;

	// Recorro el medallero.
	int i = 0;
	while(i < medallero.longitud()) {

		// Obtengo el país y las medallas de la tupla país/medallas actual.
		pair<Pais, Lista<int> > paisActual = medallero.iesimo(i);
		int oros    = paisActual.second.iesimo(0);
		int platas  = paisActual.second.iesimo(1);
		int bronces = paisActual.second.iesimo(2);

		// Guardo acá una copia de las tuplas país/medallas que ya ordené, junto con
		// la tupla país/medallas actual en la posición correcta.
		Lista<pair<Pais, Lista<int> > > nuevoOrdenado;
		bool agregado = false;

		// Recorro las tuplas país/medallas que ya ordené.
		int j = 0;
		while(j < ordenado.longitud()) {
			int orosOtro    = ordenado.iesimo(j).second.iesimo(0);
			int platasOtro  = ordenado.iesimo(j).second.iesimo(1);
			int broncesOtro = ordenado.iesimo(j).second.iesimo(2);

			// Ubico a la tupla país/medallas actual en la posición correcta dentro de la lista ordenada.
			if( !agregado &&
			   ((oros >  orosOtro) ||
			    (oros == orosOtro && platas >  platasOtro) ||
			    (oros == orosOtro && platas == platasOtro && bronces >= broncesOtro))) {
				nuevoOrdenado.agregarAtras(paisActual);
				agregado = true;
			}

			nuevoOrdenado.agregarAtras(ordenado.iesimo(j));
			j++;
		}

		// Si luego de recorrer las tuplas país/medallas ya ordenadas resulta que todavía no agregué
		// la tupla país/medallas actual, es porque la tupla actual es la que menos medallas tiene, y
		// por lo tanto su posición es al final de la lista.
		if(!agregado) nuevoOrdenado.agregarAtras(paisActual);

		// Finalmente, reemplazo el medallero ordenado actual por mi nuevo medallero ordenado, que
		// incluye a la tupla país/medallas actual.
		ordenado = nuevoOrdenado;

		i++;
	}

	return ordenado;
}

int JJOO::boicotPorDisciplina(const Categoria cat, const Pais p) {
	int i=0;
	int sacados = 0;
	Lista<Lista<Competencia> > nuevaCompetenciasPorDia;
	while(i<_competenciasPorDia.longitud()) {
		int j=0;
		Lista<Competencia> competenciasEnElDia;
		while(j<_competenciasPorDia.iesimo(i).longitud()) {
			if(_competenciasPorDia.iesimo(i).iesimo(j).categoria() == cat) {
				Competencia viejaCompe = _competenciasPorDia.iesimo(i).iesimo(j);
				Lista<Atleta> participantes = viejaCompe.participantes();
				Lista<Atleta> tempParticipantes = viejaCompe.participantes();
				string deporte = viejaCompe.categoria().first;
				Lista<int> ciaNumberDeSacados;
				Sexo sexo = viejaCompe.categoria().second;
				int h=0;
				while(h<tempParticipantes.longitud()) {
					if(tempParticipantes.iesimo(h).nacionalidad() == p) {
						participantes.sacar(tempParticipantes.iesimo(h));
						sacados++;
						ciaNumberDeSacados.agregarAtras(tempParticipantes.iesimo(h).ciaNumber());
					}
					h++;
				}
				Competencia nuevaCompe(deporte, sexo, participantes);
				if(viejaCompe.finalizada()) {
					int h = 0;
					Lista<int> ranking;
					Lista<pair<int, bool> > control;
					while(h<viejaCompe.ranking().longitud()) {
						if(!ciaNumberDeSacados.pertenece(viejaCompe.ranking().iesimo(h).ciaNumber())) {
							ranking.agregarAtras(viejaCompe.ranking().iesimo(h).ciaNumber());
						}
						h++;
					}
					h = 0;
					while(h<viejaCompe.lesTocoControlAntidoping().longitud()) {
						if(!ciaNumberDeSacados.pertenece(viejaCompe.lesTocoControlAntidoping().iesimo(h).ciaNumber())) {
							bool leToco = viejaCompe.leDioPositivo(viejaCompe.lesTocoControlAntidoping().iesimo(h));
							int suCiaNumber = viejaCompe.lesTocoControlAntidoping().iesimo(h).ciaNumber();
							pair<int, bool> par = make_pair(suCiaNumber, leToco);
							control.agregarAtras(par);
						}
						h++;
					}
					nuevaCompe.finalizar(ranking, control);
				}
				competenciasEnElDia.agregarAtras(nuevaCompe);
			}
			else {
				competenciasEnElDia.agregarAtras(_competenciasPorDia.iesimo(i).iesimo(j));
			}
			j++;
		}
		nuevaCompetenciasPorDia.agregarAtras(competenciasEnElDia);
		i++;
	}
	_competenciasPorDia = nuevaCompetenciasPorDia;
	return sacados;
}

Lista<Atleta> JJOO::losMasFracasados(const Pais p) const {
//Crea una lista con los atletas que ganaron medallas
    int h=0;
    Lista<Atleta> rank;
    while(h<competenciasFinalizadasConOroEnPodio().longitud()){
        if (competenciasFinalizadasConOroEnPodio().iesimo (h).ranking().longitud()<3){
            rank.concatenar(competenciasFinalizadasConOroEnPodio().iesimo(h).ranking());
        }else{
        	// 0, 1 y 2 en vez de 1,2,3
            rank.agregar(competenciasFinalizadasConOroEnPodio().iesimo (h).ranking().iesimo(0));
            rank.agregar(competenciasFinalizadasConOroEnPodio().iesimo (h).ranking().iesimo(1));
            rank.agregar(competenciasFinalizadasConOroEnPodio().iesimo (h).ranking().iesimo(2));
        }
        h++;
    }
//Este ciclo me da una lista de atletas del pais p que no ganaron ninguna medalla
    Lista<Atleta> atles;
	int n = 0;
    while (n < atletas().longitud()){
    	Atleta competidor = atletas().iesimo(n);
        if ((competidor.nacionalidad()== p) && !rank.pertenece(competidor)) {
        	atles.agregar(competidor);
        }
        n++;
    }
//Divido en dos casos, si la lista es vacia o si tiene al menos un elemento
    Lista<Atleta> atlesFracasados;
    if (atles.longitud()==0){
        atlesFracasados=atles;
    }
    else{
//Acá me fijo cual es el atleta que participó en mas competencias
        int k = 0, j=0;
        Atleta maxAp = atles.iesimo(k);
        Lista<Atleta> atlecomp= participantes();
        while (j < atles.longitud()){
            if (atlecomp.cantidadDeApariciones(maxAp)>atlecomp.cantidadDeApariciones(atles.iesimo(j))){
                	maxAp = atles.iesimo(j);
                }
            j++;
        }
//Acá creo la lista con todos los atletas que aparecen tantas veces como maxAp
        atlesFracasados.agregar(maxAp);
        int m=0;
        while (m < atles.longitud()){
        	//No estabas viendo si el que tenia la misma cantidad de apariciones era el mismo
            if (atlecomp.cantidadDeApariciones(maxAp)==atlecomp.cantidadDeApariciones(atles.iesimo(m)) && !(atles.iesimo(m) == maxAp)){
                atlesFracasados.agregar(atles.iesimo(m));
                }
            m++;
        }
    }
  return atlesFracasados;
}

void JJOO::liuSong(const Atleta& a, const Pais p) {
	Atleta atletaNacionalizado(a.nombre(), a.sexo(), a.anioNacimiento(), p, a.ciaNumber());
	int i = 0;
	while(i<a.deportes().longitud()) {
		atletaNacionalizado.entrenarNuevoDeporte(a.deportes().iesimo(i), a.capacidad(a.deportes().iesimo(i)));
		i++;
	}

	//Saco el atleta y lo agrego nacionalizado en atletas;
	_atletas.sacar(a);
	_atletas.agregarAtras(atletaNacionalizado);

	i=0;
	int j = 0;
	//Lista que va a reemplazar a la vieja lista de listas competencias
	Lista<Lista<Competencia> > nuevaCompetenciasPorDia;
	while(i<_competenciasPorDia.longitud()) {
		//Lista de competencias que va a reemplazar a la anterior en ese dia
		Lista<Competencia> nuevaCompetenciasEnDia;
		j = 0;
		while(j<_competenciasPorDia.iesimo(i).longitud()) {
			Competencia viejaCompe = _competenciasPorDia.iesimo(i).iesimo(j);
			//Me fijo si esa competencia tiene al atleta, si no, la dejo como estaba
			if(viejaCompe.participantes().pertenece(a)) {
				//Creo la nueva lista de participantes
				Lista<Atleta> nuevosParticipantes = viejaCompe.participantes();
				nuevosParticipantes.sacar(a);
				nuevosParticipantes.agregarAtras(atletaNacionalizado);
				//
				//Guardo la categoria
				Deporte dep = viejaCompe.categoria().first;
				Sexo sex = viejaCompe.categoria().second;
				//
				//Creo lo que va a ser la nueva competencia con el atleta cambiado
				//
				Competencia nuevaCompe(dep, sex, nuevosParticipantes);

				//Si esta finalizada, asigno el ranking y el doping
				if(viejaCompe.finalizada()) {
					int h = 0;
					Lista<int> ranking;
					Lista<pair<int, bool> > control;
					while(h<viejaCompe.ranking().longitud()) {
						ranking.agregarAtras(viejaCompe.ranking().iesimo(h).ciaNumber());
						h++;
					}
					h = 0;
					while(h<viejaCompe.lesTocoControlAntidoping().longitud()) {
						bool leToco = viejaCompe.leDioPositivo(viejaCompe.lesTocoControlAntidoping().iesimo(h));
						int suCiaNumber = viejaCompe.lesTocoControlAntidoping().iesimo(h).ciaNumber();
						pair<int, bool> par = make_pair(suCiaNumber, leToco);
						control.agregarAtras(par);
						h++;
					}
					//Finalizo
					nuevaCompe.finalizar(ranking, control);
				}
				//Agrego a lo que va a ser mi nueva lista de competencias en ese dia
				nuevaCompetenciasEnDia.agregarAtras(nuevaCompe);
			}
			else {
				nuevaCompetenciasEnDia.agregarAtras(viejaCompe);
			}
			j++;
		}
		i++;
		nuevaCompetenciasPorDia.agregarAtras(nuevaCompetenciasEnDia);
	}
	_competenciasPorDia = nuevaCompetenciasPorDia;
}

Atleta JJOO::stevenBradbury() const {
	// Guardo el atleta con menor capacidad, y la menor de sus
	// capacidades de los deportes en los que salió campeón.
	Atleta atletaMenosCapaz;
	int capacidadMenosCapaz;

	// Recorro la lista de competencias finalizadas con oro.
	int i = 0;
	while(i < competenciasFinalizadasConOroEnPodio().longitud()) {

		// Guardo el campeón de la competencia actual y
		// su capacidad en el deporte de la misma.
		Competencia competencia = competenciasFinalizadasConOroEnPodio().iesimo(i);
		Atleta campeon = competencia.ranking().cabeza();
		int capacidadCampeon = campeon.capacidad(competencia.categoria().first);

		// Si es la primera competencia que recorro, entonces tomo al campeón
		// de la misma como el atleta menos capaz hasta el momento.
		if(i == 0) {
			atletaMenosCapaz = campeon;
			capacidadMenosCapaz = capacidadCampeon;
		}

		// En caso contrario, si el campeón de esta competencia tiene una menor capacidad
		// que el atleta menos capaz hasta el momento, lo tomo como el menso capaz.
		else {
			if(capacidadCampeon < capacidadMenosCapaz) {
				atletaMenosCapaz = campeon;
				capacidadMenosCapaz = capacidadCampeon;
			}
		}

		i++;
	}

	return atletaMenosCapaz;
}

bool JJOO::uyOrdenadoAsiHayUnPatron() const {

	// Guardo acá la secuencia de mejores países.
	Lista<Pais> mejoresPaises;

	// Recorro los juegos día por día.
	int dia = 1;
	while(dia <= cantDias()) {

		// Guardo acá la cantidad de oros por país.
		Lista<pair<Pais, int> > oros;

		// Recorro el las competencias de este día.
		int i = 0;
		while(i < cronograma(dia).longitud()) {
			Competencia actual = cronograma(dia).iesimo(i);

			// Si la competencia actual está finalizada y alguien se llevó el oro,
			// voy a buscar la nacionalidad del medallista y voy a sumarle un oro a
			// ese país en mi lista de oros por país. En caso contrario, ignoro
			// esta competencia.
			if(actual.finalizada() && actual.ranking().longitud() > 0) {
				Pais pais = actual.ranking().iesimo(0).nacionalidad();

				// Busco el país del medallista en mi lista de oros por país.
				bool encontrado = false;
				int j = 0;
				while(j < oros.longitud() && !encontrado) {

					// Si encontré el país del medallista en mi lista de oros por país,
					// aumento en uno la cantidad de oros de ese país.
					if(oros.iesimo(j).first == pais) {
						pair<Pais, int> nuevaTupla = make_pair(oros.iesimo(j).first, oros.iesimo(j).second + 1);
						oros.eliminarPosicion(j);
						oros.agregarAtras(nuevaTupla);
						encontrado = true;
					}

					j++;
				}

				// Si el país del medallista no estaba en la lista de oros por país,
				// agrego ese país a la lista, con cantidad de oros uno.
				if(!encontrado) {
					oros.agregarAtras(make_pair(pais, 1));
				}
			}

			i++;
		}

		// Si hubieron oros este día, busco al mejor país y lo agrego a mejoresPaises.
		if(oros.longitud() > 0) {

			// Guardo acá el mejor país hasta el momento (tupla país/oros.)
			pair<Pais, int> mejorPais;

			// Recorro la lista de oros por país.
			i = 0;
			while(i < oros.longitud()) {

				// Si el país i-ésimo es el primero de la lista de oros, o si tiene más oros que el mejor país
				// hasta el momento, o si tiene igual cantidad de oros pero es  lexicográficamente menor,
				// entonces convierto al país i-ésimo en el mejor país hasta el momento.
				if( i == 0 ||
				    oros.iesimo(i).second > mejorPais.second ||
				   (oros.iesimo(i).second == mejorPais.second && oros.iesimo(i).first < mejorPais.first)) {
					mejorPais = oros.iesimo(i);
				}

				i++;
			}

			// Finalmente, agrego al mejor país del día a la lista de mejores países.
			mejoresPaises.agregarAtras(mejorPais.first);
		}

		dia++;
	}

	bool hayPatron = true;

	// Busco patrón si y sólo si hay tres o más mejores países.
	if(mejoresPaises.longitud() >= 3) {

		// Recorro la lista de mejores países hasta el anteúltimo elemento.
		int i = 0;
		while(i < mejoresPaises.longitud() - 1) {
			Pais actual    = mejoresPaises.iesimo(i);
			Pais siguiente = mejoresPaises.iesimo(i + 1);

			// Recorro todos los países a la derecha del país actual
			int j = i + 1;
			while(j < mejoresPaises.longitud() - 1) {

				// Si el país j-ésimo es el mismo que el actual, verifico que el patrón
				// se cumpla (el elemento siguiente siempre debe ser el mismo.)
				if(mejoresPaises.iesimo(j) == actual) {
					hayPatron = hayPatron && (mejoresPaises.iesimo(j + 1) == siguiente);
				}

				j++;
			}

			i++;
		}
	}

	return hayPatron;
}

Lista<Pais> JJOO::sequiaOlimpica() const {
    int i=0;
    Lista<Pais> paises;
    while (i < atletas().longitud()){
        if (!paises.pertenece(atletas().iesimo(i).nacionalidad())) {
            paises.agregar(atletas().iesimo(i).nacionalidad());
        }
        i++;
    }
    i=0;
    Lista<pair<Pais,int> > paisDiasSinGanar;
    while (i < paises.longitud()) {
        int y = 1;
        Lista<int> jornadas;
        jornadas.agregar(0);
        while (y < jornadaActual()){
            if (ganoMedallaEseDia(paises.iesimo(i), y)) {
                jornadas.agregarAtras(y);
            }
            y++;
        }
        jornadas.agregarAtras(jornadaActual());
        pair<Pais,int> par = make_pair (paises.iesimo(i), maxDiasSinGanar(jornadas));
        paisDiasSinGanar.agregarAtras(par);
        i++;
    }
    i=0;
    int m= paisDiasSinGanar.iesimo(0).second;
    while (i< paisDiasSinGanar.longitud()-1){
        if ( m < paisDiasSinGanar.iesimo(i+1).second) {
            m=paisDiasSinGanar.iesimo(i+1).second;
        }
        i++;
    }
    i=0;
    Lista<Pais> secos;
    while (i < paisDiasSinGanar.longitud()) {
        if (paisDiasSinGanar.iesimo(i).second==m) {
            secos.agregarAtras(paisDiasSinGanar.iesimo(i).first);
        }
        i++;
    }
    return secos;
}


int JJOO::maxDiasSinGanar(Lista<int> lista) const{
    int i = 1;
    Lista<int> list;
    while (i<lista.longitud()){
        list.agregarAtras(lista.iesimo(i)-lista.iesimo(i-1));
        i++;
    }
    i=0;
    int maximo= list.iesimo(0);
    while (i<(list.longitud()-1)){
        if (maximo<list.iesimo(i+1)) {
            maximo=list.iesimo(i+1);
        }
        i++;
    }
    return maximo;
}

bool JJOO::ganoMedallaEseDia(Pais p, int x) const{
    int i = 0;
    bool igual=false;
    while (i< cronograma(x).longitud()){
        if (cronograma(x).iesimo(i).finalizada() &&
        (((cronograma(x).iesimo(i).ranking().longitud() >= 1) && (cronograma(x).iesimo(i).ranking().iesimo(0).nacionalidad()==p)) ||
        ((cronograma(x).iesimo(i).ranking().longitud() >= 2) && (cronograma(x).iesimo(i).ranking().iesimo(1).nacionalidad()==p)) ||
        ((cronograma(x).iesimo(i).ranking().longitud() >= 3) && (cronograma(x).iesimo(i).ranking().iesimo(2).nacionalidad()==p)))) {
            igual=true;
            i= cronograma(x).longitud();
        }
        else {
            i++;
        }
    }
    return igual;
}

void JJOO::transcurrirDia() {
    int i = 0;
    //f va a ser el cronograma cambiado, con las competencias que finalizaron mas las que voy a finalizar
    Lista<Competencia> f;
    while (i < cronograma(jornadaActual()).longitud()){
        //me fijo que competencias ya finalizaron en el cronograma
        if (!((cronograma(jornadaActual()).iesimo(i)).finalizada())) {
            //guardos los datos de la competencia que tengo que finalizar
            Lista<Atleta> participan= cronograma(jornadaActual()).iesimo(i).participantes();
            Deporte dep= (cronograma(jornadaActual()).iesimo(i).categoria()).first;
            Sexo sex= (cronograma(jornadaActual()).iesimo(i).categoria()).second;
            //creo una competencia con los datos de la anterior asi la puedo finalizar
            Competencia nuevaC(dep, sex, participan);
            //la finalizo con el ranking y doping creados en los aux
            nuevaC.finalizar(rank(cronograma(jornadaActual()).iesimo(i)),doping(cronograma(jornadaActual()).iesimo(i)));
            //la agrego al cronograma nuevo
            f.agregarAtras(nuevaC);
        }
        else {
            //si la competencia ya estaba finalizada, la agrego directamente al cronograma nuevo
            f.agregarAtras(cronograma(jornadaActual()).iesimo(i));
        }
        i++;
    }
    //cambio la lista vieja de cronogramas por la nueva al JJOO
    _competenciasPorDia = m(_competenciasPorDia, f, jornadaActual());
    if(_jornadaActual != cantDias()) {
    	_jornadaActual += 1;
    }

}

//voy a hacer una lista de los cronogramas, reemplazando el de la jornada actual por el que cree
Lista<Lista<Competencia> > JJOO::m(Lista<Lista<Competencia> > h, Lista<Competencia> comp, int w){
    int i=0;
    Lista<Lista<Competencia> > f;
    while (i < h.longitud()) {
        if (i!=(w-1)) {
            f.agregarAtras(h.iesimo(i));
        }
        else {
            f.agregarAtras(comp);
        }
        i++;
    }
    return f;
}

//Propuesta de funcion para crear ranking
Lista<int> JJOO::crearRanking(const Competencia& compe) {
	//Hago una copia de los participantes
	Lista<Atleta> tempParticipantes = compe.participantes();
	Lista<int> rankingParticipantes;
	if(tempParticipantes.longitud() > 0) {
		//Voy a recorrer tempParticipantes sacando cada vez el maximo, parando cuando no haya mas para sacar
		while(tempParticipantes.longitud() > 0) {
			//Agarro el primer elemento, como ya tiene uno
			Atleta atleConMasCapacidad = tempParticipantes.iesimo(0);
			int i=1;
			//Comparo con el resto
			while(i<tempParticipantes.longitud()) {
				//Si la capacidad del deporte en esa competencia es mayor al que teniamos, lo cambiamos
				if(tempParticipantes.iesimo(i).capacidad(compe.categoria().first) >= atleConMasCapacidad
						.capacidad(compe.categoria().first)) {
					atleConMasCapacidad = tempParticipantes.iesimo(i);
				}
				i++;
			}
			//Finalmente lo agregamos al ranking y lo sacamos de la lista
			rankingParticipantes.agregarAtras(atleConMasCapacidad.ciaNumber());
			tempParticipantes.sacar(atleConMasCapacidad);
		}
	}
	return rankingParticipantes;
}

//devuelve una lista de tuplas con el cia num y capacidad para el deporte asignado
Lista<pair<int,int> > JJOO::capacidades(const Deporte d , Lista<Atleta> atle) {
    int i = 0;
    Lista<pair<int,int> > cap;
    while (i < atle.longitud()) {
        pair<int, int> par= make_pair (atle.iesimo(i).ciaNumber(),atle.iesimo(i).capacidad(d));
        cap.agregarAtras(par);
        i++;
    }
    return cap;
}
//cambio 2 elementos de lugar
Lista<pair<int,int> > JJOO::swap(Lista<pair<int,int> > lista, int a, int b){
    pair<int, int> c;
    Lista<pair<int,int> > swaped;
    int i = 0;
    while (i < lista.longitud()) {
        if ( (i==a) ) {
            swaped.agregarAtras(lista.iesimo(b));
        }
        else {
            if ( (i==b) ) {
                swaped.agregarAtras(lista.iesimo(a));
            }
            else {
                swaped.agregarAtras(lista.iesimo(i));
            }
        }
    i++;
    }
    return swaped;
}

//creo el ranking ordenado de mayor capacidad a menor
Lista<int> JJOO::rank(Competencia c){
    int m, actual = c.participantes().longitud()-1;
    Lista<int> x;
    Lista<pair<int,int> > cambiada = capacidades(c.categoria().first, c.participantes());
    while (actual >= 0) {
        m= maxPos(cambiada,0,actual);
        cambiada= swap(cambiada, actual, m);
        x.agregarAtras(cambiada.iesimo(actual).first);
        actual--;
    }
    return x;
}

//busca en que posicion esta el elemento con mayor capacidad
int JJOO::maxPos(Lista<pair<int,int> > a, int desde, int hasta) {
    int mp = desde;
    int i = desde;
    while (i < hasta) {
        i++;
        if (a.iesimo(i).second > a.iesimo(mp).second) mp = i;
    }
    return mp;
}

//creo la lista de antidoping de longitud 1, y false asi mismos(ranking(x),participantes(x)) se cumple
Lista<pair<int,bool> > JJOO::doping(const Competencia c){
    Lista<pair<int,bool> > x;
    pair<int,bool> y;
        if ((c.participantes()).longitud() >= 1 ) {
            y = make_pair (((c.participantes()).cabeza()).ciaNumber(), false);
            x.agregar(y);
        }

    return x;
}

bool JJOO::operator==(const JJOO& j) const {
	return _anio == j.anio() && _jornadaActual == j.jornadaActual() && mismosAtletas(j) && mismoCronograma(j);
}

void JJOO::mostrar(std::ostream& os) const {
	os << "Juego olimpico" << endl << "Anio: " << _anio << " Dia: " << _jornadaActual << "/" << cantDias() << endl;
	os << "Participantes:" << endl;
	int i = 0;
	while(i<_atletas.longitud()) {
		_atletas.iesimo(i).mostrar(os);
		i++;
	}
	os << endl << "Cronograma:" << endl;
	i=0;
	while(i<_competenciasPorDia.longitud()) {
		os << "Dia: " << (i+1) << "/" << cantDias() << endl;
		int j=0;
		while(j<_competenciasPorDia.iesimo(i).longitud()) {
			_competenciasPorDia.iesimo(i).iesimo(j).mostrar(os);
			os << endl;
			j++;
		}
		i++;
	}

}

void JJOO::guardar(std::ostream& os) const {
	os << "J " << _anio << " " << _jornadaActual << " [";
	int i=0;
	while(i<_atletas.longitud()) {
		os << "(";
		_atletas.iesimo(i).guardar(os);
		os << ")";
		i++;
		if(i<_atletas.longitud()) {
			os << ",";
		}
	}
	os << "] [";
	i=0;
	while(i<_competenciasPorDia.longitud()) {
		os << "[";
		int j=0;
		while(j<_competenciasPorDia.iesimo(i).longitud()) {
			os << "(";
			_competenciasPorDia.iesimo(i).iesimo(j).guardar(os);
			os << ")";
			j++;
			if(j<_competenciasPorDia.iesimo(i).longitud()) {
				os << ",";
			}
		}
		os << "]";
		i++;
		if(i<_competenciasPorDia.longitud()) {
			os << ",";
		}
	}
	os << "]";
}

void JJOO::cargar (std::istream& is) {
	char c;
	//Saco J anio jornada [
	is >> c >> _anio >> _jornadaActual >> c;
	bool loop = true;
	Lista<Atleta> nuevosAtletas;
	if(is.peek() != ']') {
		while(loop) {
			//Saco (
			is >> c;
			Atleta atle;
			atle.cargar(is);
			nuevosAtletas.agregarAtras(atle);
			//Saco )
			is >> c;
			if(is.peek() != ',') {
				loop = false;
			}
			else {
				//Saco la coma
				is >> c;
			}
		}
	}
	_atletas = nuevosAtletas;
	//Saco ] y [
	is >> c >> c;
	Lista<Lista<Competencia> > nuevasCompetenciasPorDia;
	if(is.peek() != ']') {
		loop = true;
		while(loop) {
			//Saco [
			is >> c;
			Lista<Competencia> nuevaCompetenciasEnElDia;
			if(is.peek() != ']') {
				Competencia compe;
				bool secLoop = true;
				while(secLoop) {
					//Saco (
					is >> c;
					compe.cargar(is);
					nuevaCompetenciasEnElDia.agregarAtras(compe);
					//Saco )
					is >> c;
					if(is.peek() != ',') {
						secLoop = false;
					}
					else {
						//Saco la coma
						is >> c;
					}
				}
			}
			//Saco ]
			is >> c;
			nuevasCompetenciasPorDia.agregarAtras(nuevaCompetenciasEnElDia);
			if(is.peek() != ',') {
				loop = false;
			}
			else {
				//Saco la coma
				is >> c;
			}
		}
		//Saco ]
		is >> c;
		_competenciasPorDia = nuevasCompetenciasPorDia;
	}


}

/********************************
 *          AUXILIARES          *
 ********************************/

Lista<Atleta> JJOO::participantes() const {
	Lista<Atleta> participantes;
	int i = 0;
	while (i < competencias().longitud()) {
    	participantes.concatenar(competencias().iesimo(i).participantes());
    	i++;
	}
	return participantes;
}

bool JJOO::mismosAtletas(const JJOO& j) const {
	int i = 0;
	bool igual = true;
	if(_atletas.longitud() == j.atletas().longitud()) {
		while(i<_atletas.longitud()) {
			if(!j.atletas().pertenece(_atletas.iesimo(i))) {
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

bool JJOO::mismoCronograma(const JJOO& j) const {
	bool igual = true;
	if(cantDias() == j.cantDias()) {
		int i=1;
		while(i<cantDias()) {
			if(cronograma(i) == j.cronograma(i)) {
				int y = 0;
				while(y<cronograma(i).longitud()) {
					if(!j.cronograma(i).pertenece(this->cronograma(i).iesimo(y))) {
						igual = false;
					}
				}
			}
			else {
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
