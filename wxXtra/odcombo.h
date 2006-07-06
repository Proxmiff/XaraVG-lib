// $Id$
/* @@tag:xara-cn-tp@@ THIRD PARTY COPYRIGHT */
// The following line makes normalize.pl skip type fixing
/* SKIPFIXTYPES: START */

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/odcombo.h
// Purpose:     wxOwnerDrawnComboBox and wxVListBoxPopup
// Author:      Jaakko Salli
// Modified by:
// Created:     Apr-30-2006
// RCS-ID:      
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXXTRA_ODCOMBO_H_
#define _WXXTRA_ODCOMBO_H_

#include <wx/wx.h>

#if wxUSE_ODCOMBOBOX
#undef wxXTRA_ODCOMBOBOX
#include <wx/odcombo.h>
#else
#define wxXTRA_ODCOMBOBOX 1

#include <wx/ctrlsub.h>
#include <wx/vlbox.h>
#include "combo.h"

//
// New window styles for wxOwnerDrawnComboBox
//
enum
{
    // Double-clicking cycles item if wxCB_READONLY is also used.
    wxODCB_DCLICK_CYCLES            = wxCC_SPECIAL_DCLICK,

    // If used, control itself is not custom paint using callback.
    // Even if this is not used, writable combo is never custom paint
    // until SetCustomPaintWidth is called
    wxODCB_STD_CONTROL_PAINT        = 0x1000
};


//
// Callback flags
//
enum
{
    // when set, we are painting the selected item in control,
    // not in the popup
    wxODCB_PAINTING_CONTROL           = 0x0001
};


// ----------------------------------------------------------------------------
// wxVListBoxComboPopup is a wxVListBox customized to act as a popup control.
//
// Notes:
//   wxOwnerDrawnComboBox uses this as its popup. However, it always derives
//   from native wxComboControl. If you need to use this popup with
//   wxGenericComboControl, then remember that vast majority of item manipulation
//   functionality is implemented in the wxVListBoxComboPopup class itself.
//
// ----------------------------------------------------------------------------


class WXDLLEXPORT wxVListBoxComboPopup : public wxVListBox, public wxComboPopup
{
    friend class wxOwnerDrawnComboBox;
public:

    // init and dtor
    wxVListBoxComboPopup() : wxVListBox(), wxComboPopup() { }
    virtual ~wxVListBoxComboPopup();

    // required virtuals
    virtual void Init();
    virtual bool Create(wxWindow* parent);
    virtual wxWindow *GetControl() { return this; }
    virtual void SetStringValue( const wxString& value );
    virtual wxString GetStringValue() const;

    // more customization
    virtual void OnPopup();
    virtual wxSize GetAdjustedSize( int minWidth, int prefHeight, int maxHeight );
    virtual void PaintComboControl( wxDC& dc, const wxRect& rect );
    virtual void OnComboKeyEvent( wxKeyEvent& event );
    virtual void OnComboDoubleClick();
    virtual bool LazyCreate();

    // Callbacks for drawing and measuring items. Override in a derived class for
    // owner-drawnness.
    // item: item index to be drawn, may be wxNOT_FOUND when painting combo control itself
    //       and there is no valid selection
    // flags: wxCP_PAINTING_CONTROL is set if painting to combo control instead of list
    virtual void OnDrawItem( wxDC& dc, const wxRect& rect, int item, int flags ) const;

    // Return item width, or -1 for calculating from text extent (default)
    virtual wxCoord OnMeasureItemWidth( size_t item ) const;


    // Item management
    void SetSelection( int item );
    void Insert( const wxString& item, int pos );
    int Append(const wxString& item);
    void Clear();
    void Delete( int item );
    void SetItemClientData( int n, void* clientData, wxClientDataType clientDataItemsType);
    void *GetItemClientData( int n) const;
    void SetString( int item, const wxString& str );
    wxString GetString( int item ) const;
    int GetCount() const;
    int FindString(const wxString& s) const;
    int GetSelection() const;

    //void Populate( int n, const wxString choices[] );
    void Populate( const wxArrayString& choices );
    void ClearClientDatas();

    // helpers
    int GetItemAtPosition( const wxPoint& pos ) { return HitTest(pos); }
    wxCoord GetTotalHeight() const { return EstimateTotalHeight(); }
    wxCoord GetLineHeight(int line) const { return OnGetLineHeight(line); }

	int GetWidestItemWidth() { return m_widestWidth; }

protected:

    // Called by OnComboDoubleClick and OnComboKeyEvent
    bool HandleKey( int keycode, bool saturate );

    // sends combobox select event from the parent combo control
    void SendComboBoxEvent( int selection );
    
    // gets value, sends event and dismisses
    void DismissWithEvent();

    // Re-calculates width for given item
    void CheckWidth( int pos );

    // wxVListBox implementation
    virtual void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const;
    //virtual wxCoord OnMeasureItem(size_t n) const;
    void OnDrawBackground(wxDC& dc, const wxRect& rect, size_t n) const;

    // Return item height
    virtual wxCoord OnMeasureItem( size_t item ) const;

    // filter mouse move events happening outside the list box
    // move selection with cursor
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnKey(wxKeyEvent& event);
    void OnLeftClick(wxMouseEvent& event);

    wxArrayString           m_strings;
    wxArrayPtrVoid          m_clientDatas;
    wxArrayInt              m_widths; // cached line widths

    wxFont                  m_useFont;

    //wxString                m_stringValue; // displayed text (may be different than m_strings[m_value])
    int                     m_value; // selection
    int                     m_itemHover; // on which item the cursor is
    int                     m_widestWidth; // width of widest item thus far
    int                     m_avgCharWidth;
    int                     m_baseImageWidth; // how much per item drawn in addition to text
    int                     m_itemHeight; // default item height (calculate from font size
                                          // and used in the absence of callback)
    wxClientDataType        m_clientDataItemsType;

private:

    // has the mouse been released on this control?
    bool m_clicked;

    DECLARE_EVENT_TABLE()
};


// ----------------------------------------------------------------------------
// wxOwnerDrawnComboBox: a generic wxComboBox that allows custom paint items
// in addition to many other types of customization already allowed by
// the wxComboControl.
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxOwnerDrawnComboBox : public wxComboControl, public wxItemContainer
{
    friend class wxComboPopupWindow;
    friend class wxComboControlBase;
public:

    // ctors and such
    wxOwnerDrawnComboBox() : wxComboControl() { Init(); }

    wxOwnerDrawnComboBox(wxWindow *parent,
                         wxWindowID id,
                         const wxString& value,
                         const wxPoint& pos,
                         const wxSize& size,
                         int n,
                         const wxString choices[],
                         long style = 0,
                         const wxValidator& validator = wxDefaultValidator,
                         const wxString& name = wxComboBoxNameStr)
        : wxComboControl()
    {
        Init();

        (void)Create(parent, id, value, pos, size, n,
                     choices, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxComboBoxNameStr);

    wxOwnerDrawnComboBox(wxWindow *parent,
                         wxWindowID id,
                         const wxString& value,
                         const wxPoint& pos,
                         const wxSize& size,
                         const wxArrayString& choices,
                         long style = 0,
                         const wxValidator& validator = wxDefaultValidator,
                         const wxString& name = wxComboBoxNameStr);

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0,
                const wxString choices[] = (const wxString *) NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxComboBoxNameStr);

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxComboBoxNameStr);

    virtual ~wxOwnerDrawnComboBox();

    // NULL popup can be used to indicate default interface
    virtual void SetPopupControl( wxComboPopup* popup );

    // wxControlWithItems methods
    virtual void Clear();
    virtual void Delete(int n);
    virtual int GetCount() const;
    virtual wxString GetString(int n) const;
    virtual void SetString(int n, const wxString& s);
    virtual int FindString(const wxString& s) const;
    virtual void Select(int n);
    virtual int GetSelection() const;
    void SetSelection(int n) { Select(n); }

    wxCONTROL_ITEMCONTAINER_CLIENTDATAOBJECT_RECAST

protected:

    // clears all allocated client datas
    void ClearClientDatas();

    virtual int DoAppend(const wxString& item);
    virtual int DoInsert(const wxString& item, int pos);
    virtual void DoSetItemClientData(int n, void* clientData);
    virtual void* DoGetItemClientData(int n) const;
    virtual void DoSetItemClientObject(int n, wxClientData* clientData);
    virtual wxClientData* DoGetItemClientObject(int n) const;

    // overload m_popupInterface member so we can access specific popup interface easier
    wxVListBoxComboPopup*   m_popupInterface;

    // temporary storage for the initial choices
    //const wxString*         m_baseChoices;
    //int                     m_baseChoicesCount;
    wxArrayString           m_initChs;

private:
    void Init();

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS(wxOwnerDrawnComboBox)
};


#endif // wxUSE_OWNERDRAWNCOMBOBOX

#ifdef WXXTRA_COMBO_XML_HANDLERS
class WXDLLIMPEXP_XRC wxOwnerDrawnComboBoxXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxOwnerDrawnComboBoxXmlHandler)
public:
    wxOwnerDrawnComboBoxXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
private:
    bool m_insideBox;
    wxArrayString strList;
};
#endif

#endif
    // _WX_ODCOMBO_H_

