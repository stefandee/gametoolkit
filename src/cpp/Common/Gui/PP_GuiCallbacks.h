#ifndef PP_GUICALLBACKS_H_INCLUDED
#define PP_GUICALLBACKS_H_INCLUDED

typedef std::function <void(CPIObject*)> StandardGUICallback; /**< Usual GUI elements */

typedef std::function <void(CPIObject*,int)> GUIObjectIntCallback; /**< GUI elements that send a CPIObject and an int (e.g. TrackBar) */

typedef std::function <void(CPIObject*,int,int)> GUIObjectIntIntCallback; /**< GUI elements that send CPIObject and 2x int (e.g. mouse related events) */

typedef std::function <void(CPIObject*,int,int&)> MeasureItemCallback; /**< List measure item event (perhaps it can be reused) */

#endif // PP_GUICALLBACKS_H_INCLUDED
