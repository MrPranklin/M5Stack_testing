#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <vector>
#include "NaturalSource.hpp"
#include "ArtificialSource.hpp"

class LightControl {
public:
    LightControl(std::vector<ArtificialSource *> artificials, std::vector<NaturalSource *> naturals);

    ~LightControl();

    virtual void turnEverythingOff();

    virtual void turnOnAllArtificials();

    virtual void turnOffAllArtificials();

    virtual void turnOnAllNaturals();

    virtual void turnOffAllNaturals();

    virtual void setPercentageToAllNaturals(int percentage);

    virtual void setPercentageToAllArtificials(int percentage);

    virtual void update();

protected:
    virtual void turnOnNatural(NaturalSource *n);

    virtual void turnOffNatural(NaturalSource *n);

    virtual void turnOnArtificial(ArtificialSource *a);

    virtual void turnOffArtificial(ArtificialSource *a);

    virtual void setPercentageToArtificial(ArtificialSource *a, int percentage);

    virtual void setPercentageToNatural(NaturalSource *n, int percentage);

    std::vector<ArtificialSource *> _artificials;
    std::vector<NaturalSource *> _naturals;

};

#endif //LIGHT_CONTROL_H
