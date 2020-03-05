#ifndef DATAACCESS_H
#define DATAACCESS_H


class IDataAccess
{
public:
    virtual void fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult) = 0;
    virtual void provideDataEntryToConsumer(int entryNumber) = 0;
    virtual int getNrOfDataSourceEntries() const = 0;
    virtual ~IDataAccess() = 0;
};

#endif // DATAACCESS_H
