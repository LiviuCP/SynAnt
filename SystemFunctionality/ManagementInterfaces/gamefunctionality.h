#ifndef GAMEFUNCTIONALITY_H
#define GAMEFUNCTIONALITY_H

#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../Utilities/statisticsitem.h"
#include "../Utilities/chronometer.h"

class IGameFunctionality
{
public:
    virtual ~IGameFunctionality() = 0;

    virtual void fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult) = 0;
    virtual void provideDataEntryToConsumer(int entryNumber) = 0;
    virtual int getNrOfDataSourceEntries() const = 0;

    virtual DataSourceAccessHelper* getDataSourceAccessHelper() const = 0;
    virtual WordMixer* getWordMixer() const = 0;
    virtual WordPairOwner* getWordPairOwner() const = 0;
    virtual InputBuilder* getInputBuilder() const = 0;
    virtual StatisticsItem* getStatisticsItem() const = 0;
    virtual Chronometer* getChronometer() const = 0;

    Q_SIGNAL virtual void fetchDataForPrimaryLanguageFinished(bool success, bool validEntriesFetched) = 0;
    Q_SIGNAL virtual void fetchDataForSecondaryLanguageFinished(bool success) = 0;
    Q_SIGNAL virtual void primaryLanguageDataSavingFinished(int nrOfPrimaryLanguageSavedEntries) = 0;
    Q_SIGNAL virtual void writeDataToDbErrorOccured() = 0;
    Q_SIGNAL virtual void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms) = 0;
};

Q_DECLARE_INTERFACE(IGameFunctionality, "IGameFunctionality");

#endif // GAMEFUNCTIONALITY_H
