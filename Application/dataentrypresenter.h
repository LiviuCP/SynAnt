#ifndef DATAENTRYPRESENTER_H
#define DATAENTRYPRESENTER_H

#include <QObject>
#include <QTimer>

class DataEntryFacade;
class DataProxy;

class DataEntryPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool dataEntryEnabled READ isDataEntryEnabled NOTIFY dataEntryEnabledChanged)
    Q_PROPERTY(bool addWordsPairEnabled READ isAddWordsPairEnabled NOTIFY addWordsPairEnabledChanged)
    Q_PROPERTY(bool discardAddedWordPairsEnabled READ isDiscardAddedWordPairsEnabled NOTIFY discardAddedWordPairsEnabledChanged)
    Q_PROPERTY(bool saveAddedWordPairsEnabled READ isSaveAddedWordPairsEnabled NOTIFY saveAddedWordPairsEnabledChanged)
    Q_PROPERTY(QString dataEntryPaneStatusMessage READ getDataEntryPaneStatusMessage NOTIFY dataEntryPaneStatusMessageChanged)
    Q_PROPERTY(int languageIndex READ getLanguageIndex NOTIFY languageChanged)
    Q_PROPERTY(bool dataFetchingInProgress READ isDataFetchingInProgress NOTIFY dataFetchingInProgressChanged)
    Q_PROPERTY(bool dataSavingInProgress READ isDataSavingInProgress NOTIFY dataSavingInProgressChanged)

public:
    explicit DataEntryPresenter(QObject *parent = nullptr);

    Q_INVOKABLE void handleLanguageChangeRequest(int newLanguageIndex);
    Q_INVOKABLE void handleAddWordsPairRequest(const QString& firstWord, const QString& secondWord, bool areSynonyms);
    Q_INVOKABLE void handleDiscardAddedWordPairsRequest();
    Q_INVOKABLE void handleSaveAddedWordPairsRequest();

    // call directly from game presenter to avoid unnecessary signal routing to QML
    void startDataEntry();
    void resumeDataEntry();
    void stopDataEntry();

    bool isDataEntryEnabled() const;
    bool isAddWordsPairEnabled() const;
    bool isDiscardAddedWordPairsEnabled() const;
    bool isSaveAddedWordPairsEnabled() const;
    bool isDataFetchingInProgress() const;
    bool isDataSavingInProgress() const;

    int getLanguageIndex() const;
    QString getDataEntryPaneStatusMessage() const;

signals:
    Q_SIGNAL void languageChanged();
    Q_SIGNAL void dataEntryEnabledChanged();
    Q_SIGNAL void addWordsPairEnabledChanged();
    Q_SIGNAL void discardAddedWordPairsEnabledChanged();
    Q_SIGNAL void saveAddedWordPairsEnabledChanged();
    Q_SIGNAL void dataEntryAddSucceeded();
    Q_SIGNAL void dataEntryAddInvalid();
    Q_SIGNAL void dataFetchingInProgressChanged();
    Q_SIGNAL void dataSavingInProgressChanged();
    Q_SIGNAL void dataSaveInProgress(); // used for connecting to panes outside of data entry
    Q_SIGNAL void dataEntryStopped();
    Q_SIGNAL void dataEntryPaneStatusMessageChanged();

private slots:
    void _onStatusChanged();

private:
    void _updateStatusMessage(const QString& message, int delay);
    void _updateMessage();

    DataEntryFacade* m_pDataEntryFacade;
    DataProxy* m_pDataProxy;
    QTimer* m_pStatusUpdateTimer;

    QString m_DataEntryPaneStatusMessage;
    QString m_CurrentStatusMessage;
    bool m_QuitDeferred;
};

#endif // DATAENTRYPRESENTER_H
