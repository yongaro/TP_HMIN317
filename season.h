#ifndef SEASON_H
#define SEASON_H

/**
  * \file season.h
  * \author Quentin Philippot
  * \brief Simulateur de saisons
  *
  * Fournit un ensemble de données permettant la simulation des saisons
  * et propose les quatres saisons par défaut.
  *
  */
#include <string>
#include "particle.h"

/**
* \class Season
*/
class Season {
public:
    Season();
    ~Season();
    /**
     * \fn static Season& winter ()
     * @brief Accède à la saison par défaut "Hiver"
     * @return La saison "Hiver"
     */
    static Season& winter();

    /**
     * \fn static Season& summer ()
     * @brief Accède à la saison par défaut "été"
     * @return La saison "été"
     */
    static Season& summer();

    /**
     * \fn static Season& automn ()
     * @brief Accède à la saison par défaut "Automne"
     * @return La saison "Automne"
     */
    static Season& automn();

    /**
     * \fn static Season& spring ()
     * @brief Accède à la saison par défaut "Printemps"
     * @return La saison "Printemps"
     */
    static Season& spring();

    /**
     * \fn void setName(std::string name)
     * \param name Nom à donner à la saison
     */
    void setName(std::string);


    /**
     * \fn std::string getName() const
     * \return Le nom de la saison
     */
    std::string getName() const;

    /**
     * @brief getParticle
     * @return Un pointeur vers un modèle de particule associé à la saison
     */
    Particle* getParticle();

    /**
     * @brief isInitialized est vrai si les saisons par défauts ont déjà étées initialiséess
     */
    static bool isInitialized;

    bool gotParticle() const;


private:
    /**
     * @brief staticWinter est l'instance de classe représentant la saison "Hiver"
     */
    static Season staticWinter;


    /**
     * @brief staticSpring est l'instance de classe représentant la saison "Printemps"
     */
    static Season staticSpring;


    /**
     * @brief staticSummer est l'instance de classe représentant la saison "Eté"
     */
    static Season staticSummer;


    /**
     * @brief staticAutomn est l'instance de classe représentant la saison "Automne"
     */
    static Season staticAutomn;

    /**
     * @brief initWinter paramètre la saison par défaut "Hiver"
     */
    static void initWinter();


    /**
     * @brief initSpring paramètre la saison par défaut "Printemps"
     */
    static void initSpring();


    /**
     * @brief initSummer paramètre la saison par défaut "Eté"
     */
    static void initSummer();


    /**
     * @brief initAutomn paramètre la saison par défaut "Automne"
     */
    static void initAutomn();


    /**
     * @brief initSeasons demande l'initialisations des saisons par défaut
     */
    static void initSeasons();

    /**
     * @brief name représente le nom de la saison
     */
    std::string name;

    /**
     * @brief particle est le modèle de particules associés à la saison. Il permet de faire des effets météorologiques comme la neige (Hiver) ou la pluie (Automneà
     */
    Particle particle;

};


#endif // SEASON_H
