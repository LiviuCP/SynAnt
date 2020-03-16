#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QVector>

namespace Database
{
    const QString c_DbDriverName                        {    "QSQLITE"                                                                                  };
    const QString c_DatabaseName                        {    "data.db"                                                                                  };
    const QString c_TableName                           {    "GameDataTable"                                                                            };
    const QString c_IdFieldName                         {    "rowId"                                                                                    };
    const QString c_FirstWordFieldName                  {    "firstWord"                                                                                };
    const QString c_SecondWordFieldName                 {    "secondWord"                                                                               };
    const QString c_AreSynonymsFieldName                {    "areSynonyms"                                                                              };
    const QString c_LanguageFieldName                   {    "language"                                                                                 };
    const QString c_FirstWordFieldPlaceholder           {    ":firstWord"                                                                               };
    const QString c_SecondWordFieldPlaceholder          {    ":secondWord"                                                                              };
    const QString c_AreSynonymsFieldPlaceholder         {    ":areSynonyms"                                                                             };
    const QString c_LanguageFieldPlaceholder            {    ":language"                                                                                };

    const QString c_CreateTableQuery                    {
                                                             "CREATE TABLE GameDataTable (rowId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
                                                             "firstWord TEXT, secondWord TEXT, areSynonyms INTEGER, language TEXT)"
                                                        };

    const QString c_RetrieveEntriesFromLanguageQuery    {    "SELECT * FROM GameDataTable WHERE language = '%1'"                                        };
    const QString c_InsertEntryIntoDbQuery              {
                                                             "INSERT INTO GameDataTable(firstWord, secondWord, areSynonyms, language) "
                                                             "VALUES(:firstWord, :secondWord, :areSynonyms, 'ANY')"
                                                        };
    const QString c_InsertEntryForLanguageIntoDbQuery   {
                                                             "INSERT INTO GameDataTable(firstWord, secondWord, areSynonyms, language) "
                                                             "VALUES(:firstWord, :secondWord, :areSynonyms, :language)"
                                                        };

    const QVector<QString> c_LanguageCodes              {
                                                             "EN",  // English
                                                             "DE",  // German
                                                             "RO",  // Romanian
                                                             "IT",  // Italian
                                                             "ES",  // Spanish
                                                             "PT",  // Portuguese
                                                             "MY",  // Hungarian
                                                             "TR",  // Turkish
                                                        };
}

#endif // DATABASE_H
