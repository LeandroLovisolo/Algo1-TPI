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
    return _competenciasPorDia.iesimo(dia);
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
            if ((competencias().iesimo(i)).finalizada() && ((competencias().iesimo(i)).ranking()).longitud() > 0 )
                {comp.agregar(competencias().iesimo(i));
                }
            i++;
        }
	return comp;
}

Lista<Atleta> JJOO::dePaseo() const {
    int i=0;
    Lista<Atleta> atletas;
        while (i < _atletas.longitud()) {
            if (participantesJJOO(competencias()).pertenece(_atletas.iesimo(i))) {
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
void JJOO::liuSong(const Atleta& a, const Pais p) {

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

}
void JJOO::guardar(std::ostream& os) const {

}
void JJOO::cargar (std::istream& is) {

}
