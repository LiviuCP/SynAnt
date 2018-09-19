#ifndef STYLES_H
#define STYLES_H

#include "../../Common/gamestrings.h"

static const QString c_QLabelStyleString                        {    "QLabel,"                                                                                  };
static const QString c_SelectableLabelStyleString               {    "SelectableLabel,"                                                                         };
static const QString c_QGroupBoxStyleString                     {    "QGroupBox{background-color: %1; color: %2; border: 1px solid %3;}"                        };
static const QString c_QPushButtonStyleString                   {    "QPushButton{background-color: %1; color: %2;}"                                            };
static const QString c_QRadioButtonStyleString                  {    "QRadioButton{background-color: %1; color: %2;}"                                           };
static const QString c_QToolTipStyleString                      {    "QToolTip{}"                                                                               };
static const QString c_IntroductionWindowStyleString            {    "IntroductionWindow{background-color: %1;}"                                                };
static const QString c_HintsWindowStyleString                   {    "HintsWindow{background-color: %1;}"                                                       };
static const QString c_MainGameWindowStyleString                {    "MainGameWindow{background-color: %1;}"                                                    };
static const QString c_FatalErrorsWindowStyleString             {    "FatalErrors{background-color: %1;}"                                                       };

static const QString c_WordPieceNotSelectedStyleString          {    "color: %1; border-width: 1px; background-color: %2;"                                      };
static const QString c_WordPieceSelectedStyleString             {    "color: %1; border-width: 1px; background-color: %2;"                                      };
static const QString c_WordFirstPieceNotSelectedStyleString     {    "color: %1; border-width: 1px; background-color: %2;"                                      };
static const QString c_WordLastPieceNotSelectedStyleString      {    "color: %1; border-width: 1px; background-color: %2;"                                      };
static const QString c_WordFirstPieceSelectedStyleString        {    "color: %1; border-width: 1px; background-color: %2;"                                      };
static const QString c_WordLastPieceSelectedStyleString         {    "color: %1; border-width: 1px; background-color: %2;"                                      };
static const QString c_ErrorMessageStyleString                  {    "color: %1; border-width: 1px; background-color: %2;"                                      };


namespace Styles {

    const QString c_ApplicationStyle                            {    c_QLabelStyleString                                                                        +
                                                                     c_SelectableLabelStyleString                                                               +
                                                                     c_QGroupBoxStyleString
                                                                         .arg(GameStrings::c_BackgroundColor)
                                                                         .arg(GameStrings::c_TextColor)
                                                                         .arg(GameStrings::c_BorderColor)                                                       +
                                                                     c_QPushButtonStyleString
                                                                         .arg(GameStrings::c_PushButtonColor)
                                                                         .arg(GameStrings::c_TextColor)                                                         +
                                                                     c_QRadioButtonStyleString
                                                                         .arg(GameStrings::c_BackgroundColor)
                                                                         .arg(GameStrings::c_TextColor)                                                         +
                                                                    c_QToolTipStyleString                                                                       +
                                                                    c_IntroductionWindowStyleString
                                                                         .arg(GameStrings::c_BackgroundColor)                                                   +
                                                                    c_HintsWindowStyleString
                                                                         .arg(GameStrings::c_BackgroundColor)                                                   +
                                                                    c_MainGameWindowStyleString
                                                                         .arg(GameStrings::c_BackgroundColor)                                                   +
                                                                    c_FatalErrorsWindowStyleString
                                                                         .arg(GameStrings::c_BackgroundColor)
                                                                };

    const QString c_WordPieceNotSelectedStyle                   {    c_WordPieceNotSelectedStyleString
                                                                         .arg(GameStrings::c_TextColor)
                                                                         .arg(GameStrings::c_BackgroundColor)
                                                                };

    const QString c_WordPieceSelectedStyle                      {    c_WordPieceSelectedStyleString
                                                                         .arg(GameStrings::c_TextColor)
                                                                         .arg(GameStrings::c_WordPieceSelectedBackgroundColor)
                                                                };

    const QString c_WordFirstPieceNotSelectedStyle              {    c_WordFirstPieceNotSelectedStyleString
                                                                         .arg(GameStrings::c_WordFirstPieceTextColor)
                                                                         .arg(GameStrings::c_BackgroundColor)
                                                                };

    const QString c_WordLastPieceNotSelectedStyle               {    c_WordLastPieceNotSelectedStyleString
                                                                         .arg(GameStrings::c_WordLastPieceTextColor)
                                                                         .arg(GameStrings::c_BackgroundColor)
                                                                };

    const QString c_WordFirstPieceSelectedStyle                 {    c_WordFirstPieceSelectedStyleString
                                                                         .arg(GameStrings::c_WordFirstPieceTextColor)
                                                                         .arg(GameStrings::c_WordPieceSelectedBackgroundColor)
                                                                };

    const QString c_WordLastPieceSelectedStyle                  {    c_WordLastPieceSelectedStyleString
                                                                         .arg(GameStrings::c_WordLastPieceTextColor)
                                                                         .arg(GameStrings::c_WordPieceSelectedBackgroundColor)
                                                                };

    const QString c_ErrorMessageStyle                           {    c_ErrorMessageStyleString
                                                                         .arg(GameStrings::c_FatalErrorTextColor)
                                                                         .arg(GameStrings::c_BackgroundColor)
                                                                };
}

#endif // STYLES_H
