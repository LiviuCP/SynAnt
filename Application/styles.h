#ifndef STYLES_H
#define STYLES_H

namespace Styles {
    const QString c_ApplicationStyle                    {    "QLabel,"
                                                             "QGroupBox{background-color: grey; color: white; border: 1px solid white;}"
                                                             "QPushButton{background-color: darkgrey; color: white;}"
                                                             "QRadioButton{background-color: grey; color: white;}"
                                                             "QToolTip{background-color: grey; color: yellow; border: 1px solid yellow;}"
                                                             "IntroductionWindow,"
                                                             "HintsWindow,"
                                                             "MainGameWindow,"
                                                             "FatalErrors{background-color: grey;}"
                                                        };

    const QString c_WordBeginPieceNotSelectedStyle      {    "color: blue; border-width: 1px; background-color: grey;"                                  };
    const QString c_WordEndPieceNotSelectedStyle        {    "color: red; border-width: 1px; background-color: grey;"                                   };

    const QString c_WordBeginPieceSelectedStyle         {    "color: blue; border-width: 1px; background-color: black;"                                 };
    const QString c_WordEndPieceSelectedStyle           {    "color: red; border-width: 1px; background-color: black;"                                  };
    }

#endif // STYLES_H
