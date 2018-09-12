#ifndef STYLES_H
#define STYLES_H

#include "../../Common/gamestrings.h"

namespace Styles {

    const QString c_ApplicationStyleQLabel              {    "QLabel,"                                                                                  };
    const QString c_ApplicationStyleSelectableLabel     {    "SelectableLabel,"                                                                         };
    const QString c_ApplicationStyleQGroupBox           {    "QGroupBox{background-color: %1; color: %2; border: 1px solid %3;}"                        };
    const QString c_ApplicationStyleQPushButton         {    "QPushButton{background-color: %1; color: %2;}"                                            };
    const QString c_ApplicationStyleQRadioButton        {    "QRadioButton{background-color: %1; color: %2;}"                                           };
    const QString c_ApplicationStyleQToolTip            {    "QToolTip{}"                                                                               };
    const QString c_ApplicationStyleIntroductionWindow  {    "IntroductionWindow{background-color: %1;}"                                                };
    const QString c_ApplicationStyleHintsWindow         {    "HintsWindow{background-color: %1;}"                                                       };
    const QString c_ApplicationStyleMainGameWindow      {    "MainGameWindow{background-color: %1;}"                                                    };
    const QString c_ApplicationStyleFatalErrors         {    "FatalErrors{background-color: %1;}"                                                       };

    const QString c_ApplicationStyle                    {    c_ApplicationStyleQLabel                                                                   +
                                                             c_ApplicationStyleSelectableLabel                                                          +
                                                             c_ApplicationStyleQGroupBox
                                                                 .arg(GameStrings::c_BackgroundColor)
                                                                 .arg(GameStrings::c_TextColor)
                                                                 .arg(GameStrings::c_BorderColor)                                                       +
                                                             c_ApplicationStyleQPushButton
                                                                 .arg(GameStrings::c_PushButtonColor)
                                                                 .arg(GameStrings::c_TextColor)                                                         +
                                                             c_ApplicationStyleQRadioButton
                                                                 .arg(GameStrings::c_BackgroundColor)
                                                                 .arg(GameStrings::c_TextColor)                                                         +
                                                             c_ApplicationStyleQToolTip                                                                 +
                                                             c_ApplicationStyleIntroductionWindow
                                                                 .arg(GameStrings::c_BackgroundColor)                                                   +
                                                             c_ApplicationStyleHintsWindow
                                                                 .arg(GameStrings::c_BackgroundColor)                                                   +
                                                             c_ApplicationStyleMainGameWindow
                                                                 .arg(GameStrings::c_BackgroundColor)                                                   +
                                                             c_ApplicationStyleFatalErrors
                                                                 .arg(GameStrings::c_BackgroundColor)
                                                        };

    const QString c_WordBeginPieceNotSelectedStyle      {    QString{"color: %1; border-width: 1px; background-color: %2;"}
                                                                 .arg(GameStrings::c_WordFirstPieceTextColor)
                                                                 .arg(GameStrings::c_BackgroundColor)
                                                        };

    const QString c_WordEndPieceNotSelectedStyle        {    QString{"color: %1; border-width: 1px; background-color: %2;"}
                                                                 .arg(GameStrings::c_WordLastPieceTextColor)
                                                                 .arg(GameStrings::c_BackgroundColor)
                                                        };

    const QString c_WordBeginPieceSelectedStyle         {    QString{"color: %1; border-width: 1px; background-color: %2;"}
                                                                 .arg(GameStrings::c_WordFirstPieceTextColor)
                                                                 .arg(GameStrings::c_WordPieceSelectedBackgroundColor)
                                                        };

    const QString c_WordEndPieceSelectedStyle           {    QString{"color: %1; border-width: 1px; background-color: %2;"}
                                                                 .arg(GameStrings::c_WordLastPieceTextColor)
                                                                 .arg(GameStrings::c_WordPieceSelectedBackgroundColor)
                                                        };

    const QString c_ErrorMessageStyle                   {    QString{"color: %1; border-width: 1px; background-color: %2;"}
                                                                 .arg(GameStrings::c_FatalErrorTextColor)
                                                                 .arg(GameStrings::c_BackgroundColor)
                                                        };
}

#endif // STYLES_H
