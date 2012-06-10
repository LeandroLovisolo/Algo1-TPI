#include <iostream>
#include "tipos.h"
#include "lista.h"
#include "jjoo.h"

JJOO::JJOO () {}
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
	return 1;
}

Lista<Atleta> JJOO::losMasFracasados(const Pais p) const {
	return Lista<Atleta>();
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
