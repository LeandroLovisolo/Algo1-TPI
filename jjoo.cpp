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

	// Recorro la lista de países.
	int i = 0;
	while(i < paises.longitud()) {
		Pais pais = paises.iesimo(i);
		int oros = 0;
		int platas = 0;
		int bronces = 0;

		// Recorro los rankings de todas las competencias y acumulo las medallas del país actual.
		int j = 0;
		while(j < competencias().longitud()) {
			Lista<Atleta> ranking = competencias().iesimo(j).ranking();
			if(ranking.longitud() > 0 && ranking.iesimo(0).nacionalidad() == pais) oros++;
			if(ranking.longitud() > 1 && ranking.iesimo(1).nacionalidad() == pais) platas++;
			if(ranking.longitud() > 2 && ranking.iesimo(2).nacionalidad() == pais) bronces++;
			j++;
		}

		// Si el país ganó alguna medalla, agrego al medallero la tupla país/medallas.
		if(oros + platas + bronces > 0) {
			Lista<int> medallas;
			medallas.agregarAtras(oros);
			medallas.agregarAtras(platas);
			medallas.agregarAtras(bronces);
			medallero.agregar(make_pair(pais, medallas));
		}

		i++;
	}

	// Devuelvo el medallero ordenado
	return ordenarMedallero(medallero);
}

Lista<Pais> JJOO::paises() const {
	Lista<Pais> paises;
	int i = 0;
	while(i < _atletas.longitud()) {
		Pais actual = _atletas.iesimo(i).nacionalidad();
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

		// Obtengo la tupla país/medallas actual y las medallas (para más legibilidad.)
		pair<Pais, Lista<int> > tuplaActual = medallero.iesimo(i);
		int oros    = tuplaActual.second.iesimo(0);
		int platas  = tuplaActual.second.iesimo(1);
		int bronces = tuplaActual.second.iesimo(2);

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
				nuevoOrdenado.agregarAtras(tuplaActual);
				agregado = true;
			}

			// Dejo la tupla que ya había ordenado en la posición que le corresponde.
			nuevoOrdenado.agregarAtras(ordenado.iesimo(j));
			j++;
		}

		// Si luego de recorrer las tuplas país/medallas ya ordenadas resulta que todavía no agregué
		// la tupla país/medallas actual, es porque la tupla actual es la que menos medallas tiene, y
		// por lo tanto su posición es al final de la lista.
		if(!agregado) nuevoOrdenado.agregarAtras(tuplaActual);

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
    Lista<Pais> paises = this->paises();
    
	// Guardo acá una lista de tuplas país/máxima cantidad de días que pasaron sin ganar medallas.
	Lista<pair<Pais,int> > paisDiasSinGanar;

	// Recorro todos los países.
    int i = 0;
    while (i < paises.longitud()) {
    	Pais pais = paises.iesimo(i);
    	
    	// Armo una lista con las jornadas ganadoras de ese país.
        Lista<int> jornadasGanadoras;

        // Agrego un cero al principio de la lista para poder calcular
        // las diferencias entre días (como en la especificación.)
        jornadasGanadoras.agregar(0);
        
        // Recorro las jornadas hasta la actual, excluyéndola.
        int j = 1;
        while (j < jornadaActual()){

        	// Si el país ganó alguna medalla en la jornada
        	// actual, agrego la jornada a la lista.
            if(ganoMedallasEseDia(pais, j)) {
                jornadasGanadoras.agregarAtras(j);
            }
            j++;
        }
        
        // Agrego la jornada actual al final de la lista, para poder calcular
        // las diferencias entre días (otra vez, como en la especificación.)
        jornadasGanadoras.agregarAtras(jornadaActual());

        // Calculo la máxima diferencia entre días, y agrego
        // la tupla país/diferencia de días a la lista.
        paisDiasSinGanar.agregarAtras(make_pair(pais, maximaDistanciaEntreJornadas(jornadasGanadoras)));

        i++;
    }

    // Busco la máxima cantidad de días que algún país pasó sin ganar medallas.
    int maximosDiasSinGanar;
    i = 0;
    while(i < paisDiasSinGanar.longitud()) {
    	if(i == 0 || paisDiasSinGanar.iesimo(i).second > maximosDiasSinGanar) {
    		maximosDiasSinGanar = paisDiasSinGanar.iesimo(i).second;
    	}
    	i++;
    }

    // Me quedo con los países cuya mayor cantidad de días sin ganar medallas
    // es mayor o igual que la del resto de los países.
    Lista<Pais> secos;
    i = 0;
    while (i < paisDiasSinGanar.longitud()) {
        if(paisDiasSinGanar.iesimo(i).second == maximosDiasSinGanar) {
            secos.agregar(paisDiasSinGanar.iesimo(i).first);
        }
        i++;
    }

    return secos;
}

int JJOO::maximaDistanciaEntreJornadas(Lista<int> jornadas) const {
	// Puedo asumir que recibo dos o más jornadas.

	// Guardo acá las distancias calculadas.
    Lista<int> distancias;

    // Calculo todas las distancias entre jornadas.
    int i = 1;
    while (i < jornadas.longitud()){
        distancias.agregarAtras(jornadas.iesimo(i) - jornadas.iesimo(i - 1));
        i++;
    }

    // Busco la máxima distancia.
    int maximaDistancia;
    i = 0;
    while(i < distancias.longitud()) {
    	if(i == 0 || distancias.iesimo(i) > maximaDistancia) {
    		maximaDistancia = distancias.iesimo(i);
    	}
    	i++;
    }

    return maximaDistancia;
}

bool JJOO::ganoMedallasEseDia(Pais pais, int dia) const{
    bool gano = false;

    // Recorro el cronograma del día.
    int i = 0;
    while(i < cronograma(dia).longitud()) {
    	Competencia competencia = cronograma(dia).iesimo(i);

    	// Recorro el ranking de la competencia actual hasta el tercer puesto.
    	int j = 0;
    	while(j < competencia.ranking().longitud() && j < 3) {

    		// El valor de retorno es true sólo si el país ganó alguna medalla.
    		gano = gano || competencia.ranking().iesimo(j).nacionalidad() == pais;
    		j++;
    	}

    	i++;
    }

    return gano;
}

void JJOO::transcurrirDia() {
	// Guardo acá el nuevo cronograma de la jornada actual.
    Lista<Competencia> nuevoCronogramaJornadaActual;

    // Recorro las competencias de la jornada actual.
    int i = 0;
    while(i < cronograma(jornadaActual()).longitud()) {
    	Competencia competencia = cronograma(jornadaActual()).iesimo(i);

        // Si la competencia no estaba finalizada, la finalizo.
        if (!competencia.finalizada()) {
        	competencia = finalizarCompetencia(competencia);
        }

        // Agrego competencia al nuevo cronograma.
		nuevoCronogramaJornadaActual.agregarAtras(competencia);
        i++;
    }

    // Reemplazo el cronograma de la jornada actual por el nuevo cronograma finalizado.
    reemplazarCronogramaJornadaActual(nuevoCronogramaJornadaActual);

    // Transcurro el día.
    _jornadaActual++;
}

Competencia JJOO::finalizarCompetencia(const Competencia& competencia) const  {
	// Creo una copia de la competencia.
	Competencia finalizada(competencia.categoria().first,
			               competencia.categoria().second,
			               competencia.participantes());

	// Finalizo la competencia generando un ranking y un control antidoping.
	finalizada.finalizar(generarRanking(competencia), generarAntidoping(competencia));

    return finalizada;
}

Lista<int> JJOO::generarRanking(const Competencia& competencia) const {
	// Guardo acá el ranking construído.
	Lista<int> ranking;

	Lista<Atleta> participantesSinRankear = competencia.participantes();

	// Rankeo todos los participantes hasta quedarme sin participantes para rakear.
	while(participantesSinRankear.longitud() > 0) {

		// Busco el participante con menos capacidad entre los que aún no fueron rankeados.
		Atleta peorParticipante;
		int i = 0;
		while(i < participantesSinRankear.longitud()) {
			Deporte deporte = competencia.categoria().first;
			Atleta participante = participantesSinRankear.iesimo(i);

			// En caso de ser el peor hasta el momento, me quedo con el participante i-ésimo.
			if(i == 0 || participante.capacidad(deporte) < peorParticipante.capacidad(deporte)) {
				peorParticipante = participante;
			}

			i++;
		}

		// Agrego el peor participante hallado al principio del ranking.
		ranking.agregar(peorParticipante.ciaNumber());

		// Lo elimino de la lista de participantes sin rankear.
		participantesSinRankear.eliminarPosicion(participantesSinRankear.posicion(peorParticipante));
	}

	return ranking;
}

Lista<pair<int,bool> > JJOO::generarAntidoping(const Competencia& competencia) const {
    Lista<pair<int,bool> > antidoping;

    // Tomo al primer participante y lo agrego al control, con resultado falso.
	if(competencia.participantes().longitud() > 0) {
		antidoping.agregar(make_pair(competencia.participantes().cabeza().ciaNumber(), false));
	}

    return antidoping;
}

void JJOO::reemplazarCronogramaJornadaActual(Lista<Competencia> nuevoCronograma) {
	Lista<Lista<Competencia> > nuevasCompetenciasPorDia;

	// Recorro todas las jornadas.
    int i = 0;
    while (i < _competenciasPorDia.longitud()) {

    	// Si la i-ésima jornada es la actual, reemplazo
    	// su cronograma por el recibido como parámetro.
    	if(i == _jornadaActual - 1) {
    		nuevasCompetenciasPorDia.agregarAtras(nuevoCronograma);
    	} else {
    		nuevasCompetenciasPorDia.agregarAtras(_competenciasPorDia.iesimo(i));
    	}

        i++;
    }

    // Reemplazo por las nuevas competencias.
    _competenciasPorDia = nuevasCompetenciasPorDia;
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

// Devuelve una lista de atletas donde cada atleta aparece tantas
// veces como la cantidad de competencias en las que participa.
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
