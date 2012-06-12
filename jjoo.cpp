#include <iostream>
#include "tipos.h"
#include "lista.h"
#include "jjoo.h"

JJOO::JJOO () {
	_jornadaActual = 1;
	Lista<Competencia> listaCompetencias;
	_competenciasPorDia.agregarAtras(listaCompetencias);
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
    return _competenciasPorDia.longitud ();
}
int JJOO::jornadaActual() const {
    return _jornadaActual;
}
Lista<Competencia> JJOO::cronograma(const int dia) const {
    return _competenciasPorDia.iesimo(dia-1);
}
Lista<Competencia> JJOO::competencias() const {
    int i = 0;
    Lista<Competencia> competencias;
        while (i<cantDias()) {
            competencias.concatenar(_competenciasPorDia.iesimo(i));
            i++;
        }
    return competencias;
}

Lista<Competencia> JJOO::competenciasFinalizadasConOroEnPodio() const {
	int i = 0;
	Lista<Competencia> comp;
        while (i < competencias().longitud())   {
            if ((competencias().iesimo(i)).finalizada() && ((competencias().iesimo(i)).ranking()).longitud() > 0 ) {
            	comp.agregar(competencias().iesimo(i));
            }
            i++;
        }
	return comp;
}

Lista<Atleta> JJOO::dePaseo() const {
    int i=0;
    Lista<Atleta> atletas;
        while (i< _atletas.longitud()) {
        	//Si no alguno de los que esta en la lista de atletas del JJOO no pertenece a la lista de participantes lo agrega
        	//Faltaba la negacion(!)
            if (!participantesJJOO(competencias()).pertenece(_atletas.iesimo(i))) {
                atletas.agregar(_atletas.iesimo(i));
            }
            i++;
        }
	return atletas;
}

Lista<pair<Pais,Lista<int> > > JJOO::medallero() const {
	return Lista<pair<Pais,Lista<int> > >();
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
            rank.agregar(competenciasFinalizadasConOroEnPodio().iesimo (h).ranking().iesimo(1));
            rank.agregar(competenciasFinalizadasConOroEnPodio().iesimo (h).ranking().iesimo(2));
            rank.agregar(competenciasFinalizadasConOroEnPodio().iesimo (h).ranking().iesimo(3));
        }
        h++;
    }

//Este ciclo me da una lista de atletas del pais p que no ganaron ninguna medalla
    Lista<Atleta> atles;
	int n = 0;
	Atleta competidores = (atletas().iesimo(n));
    while (n < atletas().longitud()){
        if ((competidores.nacionalidad()== p)&& !(rank.pertenece(competidores))) {
            atles.agregar(competidores);
          }
          n++;
        }
//Divido en dos casos, si la lista es vacia o si tiene al menos un elemento
    Lista<Atleta> atlesFracasados;
    if (atles.longitud()==0){
        atlesFracasados=atles;
    }else{
//Acá me fijo cual es el atleta que participó en mas competencias
        int k = 0, j=0;
        Atleta maxAp = atles.iesimo(k);
        Lista<Atleta> atlecomp= participantesJJOO(competencias());
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
            if (atlecomp.cantidadDeApariciones(maxAp)==atlecomp.cantidadDeApariciones(atles.iesimo(m))){
                atlesFracasados.agregar(atles.iesimo(m));
                }
            m++;
        }
    }
  return atlesFracasados;
}

//Por que se crea toda la lista otra vez? porque no hay funciones publicas que permitan modificar atletas en competencias, incluso
//iesimo devuelve una constante, no queda otra
void JJOO::liuSong(const Atleta& a, const Pais p) {
	Atleta atletaNacionalizado(a.nombre(), a.sexo(), a.anioNacimiento(), p, a.ciaNumber());
	int i = 0;
	while(i<a.deportes().longitud()) {
		atletaNacionalizado.entrenarNuevoDeporte(a.deportes().iesimo(i), a.capacidad(a.deportes().iesimo(i)));
		i++;
	}

	atletaNacionalizado.mostrar(cout);

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
			//Me fijo si esa competencia tiene al atleta, si no, la dejo como estaba
			if(_competenciasPorDia.iesimo(i).iesimo(j).participantes().pertenece(a)) {
				Competencia viejaCompe = _competenciasPorDia.iesimo(i).iesimo(j);
				//Creo la nueva lista de participantes
				Lista<Atleta> nuevosParticipantes = viejaCompe.participantes();
				nuevosParticipantes.sacar(a);
				nuevosParticipantes.agregarAtras(atletaNacionalizado);
				//
				//Guardo la categoria
				Deporte dep = _competenciasPorDia.iesimo(i).iesimo(j).categoria().first;
				Sexo sex = _competenciasPorDia.iesimo(i).iesimo(j).categoria().second;
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
						ranking.agregarAtras(_competenciasPorDia.iesimo(i).iesimo(j).ranking().iesimo(h).ciaNumber());
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
				nuevaCompetenciasEnDia.agregarAtras(_competenciasPorDia.iesimo(i).iesimo(j));
			}
			j++;
		}
		i++;
		nuevaCompetenciasPorDia.agregarAtras(nuevaCompetenciasEnDia);
	}
	_competenciasPorDia = nuevaCompetenciasPorDia;
}
Atleta JJOO::stevenBradbury() const {
	return Atleta();
}
bool JJOO::uyOrdenadoAsiHayUnPatron() const {
	return true;
}
Lista<Pais> JJOO::sequiaOlimpica() const {
	return Lista<Pais>();
}
void JJOO::transcurrirDia() {
	return;
}

bool JJOO::operator==(const JJOO& j) const {
	return true;
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

//AUX
Lista<Atleta> JJOO::participantesJJOO(const Lista<Competencia> competencias) const {
	int i=0;
	Lista<Atleta> participantes;
	while (i < competencias.longitud()) {
    	participantes.concatenar(competencias.iesimo(i).participantes());
    	i++;
	}
	return participantes;
}
