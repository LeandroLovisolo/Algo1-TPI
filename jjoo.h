#ifndef JJOO_H
#define JJOO_H

#include "competencia.h"

typedef pair<int, pair<int, int> > infoM;

class JJOO{
    public:

        JJOO();
        // |cronograma|==cantDias. Si un día no hay competencias, que esté la lista vacía.
        JJOO(const int anio, const Lista<Atleta>& atletas, const Lista<Lista<Competencia> >& competenciasPorDia);
        int anio() const;
        Lista<Atleta> atletas() const;
        int cantDias() const;
        int jornadaActual() const;
        Lista<Competencia> cronograma(const int dia) const;

        Lista<Competencia> competencias() const;
        Lista<Competencia> competenciasFinalizadasConOroEnPodio() const;

        Lista<Atleta> dePaseo() const;
        Lista<pair<Pais,Lista<int> > > medallero() const;
        int boicotPorDisciplina(const Categoria cat, const Pais p);
        Lista<Atleta> losMasFracasados(const Pais p) const;
        void liuSong(const Atleta& a, const Pais p);
        Atleta stevenBradbury() const;
        bool uyOrdenadoAsiHayUnPatron() const;
        Lista<Pais> sequiaOlimpica() const;
        void transcurrirDia();

        bool operator==(const JJOO& j) const;

        void mostrar(std::ostream& os) const;
        void guardar(std::ostream& os) const;
        void cargar (std::istream& is);

    private:
        int _anio;
        Lista<Atleta> _atletas;
        int _jornadaActual;
        Lista<Lista<Competencia> > _competenciasPorDia;         // En la i-ésima posición de la lista, las competencias del día i+1.

        enum {ENCABEZADO_ARCHIVO = 'J'};

        // Auxiliares generales
        Lista<Pais> paises() const;
        Lista<Atleta> participantes() const;

        // Auxiliares JJOO::medallero()
        Lista<pair<Pais,Lista<int> > > ordenarMedallero(const Lista<pair<Pais,Lista<int> > > &) const;

        // Auxiliares JJOO::sequiaOlimpica()
        bool ganoMedallasEseDia(Pais p, int x) const;
        int maximaDistanciaEntreJornadas(Lista<int> jornadas) const;

        // Auxiliares JJOO::transcurrirDia()
        Competencia finalizarCompetencia(const Competencia& competencia) const;
        Lista<int> generarRanking(const Competencia& competencia) const;
        Lista<pair<int, bool> > generarAntidoping(const Competencia& competencia) const;
        void reemplazarCronogramaJornadaActual(Lista<Competencia> nuevoCronograma);

        // Auxiliares JJOO::operator==()
        bool mismosAtletas(const JJOO& c) const;
        bool mismoCronograma(const JJOO& j) const;
};

std::ostream & operator<<(std::ostream & os,const JJOO & j);

#endif // JJOO_H

