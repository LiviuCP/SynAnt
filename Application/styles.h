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

    const QString c_WordBeginPieceStyle                 {    "color: blue; border-width: 1px;"                                                          };
    const QString c_WordEndPieceStyle                   {    "color: red; border-width: 1px;"                                                           };
    const QString c_WordEachPieceStyle                  {    "border-width:1px"                                                                         };
}

#endif // STYLES_H
