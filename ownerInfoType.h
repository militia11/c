#ifndef OWNERINFOTYPE_H
#define OWNERINFOTYPE_H

#define ownerDefaultText "Proszę wpisać..."
struct ownerInfoType
{
    ownerInfoType()
        : ownerName(ownerDefaultText)
        , ownerPhone(ownerDefaultText)
        , ownerCity(ownerDefaultText)
        , ownerStreet(ownerDefaultText)
    {
    }

    QString ownerName;
    QString ownerPhone;
    QString ownerCity;
    QString ownerStreet;
};

#endif // OWNERINFOTYPE_H
