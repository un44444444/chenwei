/////////////////////////////////////////////////////////////////////////////
// Name:        treetest.cpp
// Purpose:     wxTreeCtrl sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: treetest.cpp 44769 2007-03-11 23:29:26Z PC $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/log.h"
#endif

#include "wx/colordlg.h"
#include "wx/numdlg.h"

#ifdef __WIN32__
    // this is not supported by native control
    #define NO_VARIABLE_HEIGHT
#endif

#include "wx/treectrl.h"

#include "treetest.h"
#include "mytreectrl.h"

#ifndef __WXMSW__
    #include "../sample.xpm"
#endif

// verify that the item is ok and insult the user if it is not
#define CHECK_ITEM( item ) if ( !item.IsOk() ) {                                 \
                             wxMessageBox(wxT("Please select some item first!"), \
                                          wxT("Tree sample error"),              \
                                          wxOK | wxICON_EXCLAMATION,             \
                                          this);                                 \
                             return;                                             \
                           }

#define MENU_LINK(name) EVT_MENU(TreeTest_##name, MyFrame::On##name)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_IDLE(MyFrame::OnIdle)
    EVT_SIZE(MyFrame::OnSize)

    MENU_LINK(Quit)
    MENU_LINK(About)

    MENU_LINK(TogButtons)
    MENU_LINK(TogTwist)
    MENU_LINK(TogLines)
    MENU_LINK(TogEdit)
    MENU_LINK(TogHideRoot)
    MENU_LINK(TogRootLines)
    MENU_LINK(TogBorder)
    MENU_LINK(TogFullHighlight)
    MENU_LINK(SetFgColour)
    MENU_LINK(SetBgColour)
    MENU_LINK(ResetStyle)

    MENU_LINK(Highlight)
    MENU_LINK(Dump)
#ifndef NO_MULTIPLE_SELECTION
    MENU_LINK(DumpSelected)
    MENU_LINK(Select)
    MENU_LINK(Unselect)
    MENU_LINK(ToggleSel)
#endif // NO_MULTIPLE_SELECTION
    MENU_LINK(Rename)
    MENU_LINK(Count)
    MENU_LINK(CountRec)
    MENU_LINK(Sort)
    MENU_LINK(SortRev)
    MENU_LINK(SetBold)
    MENU_LINK(ClearBold)
    MENU_LINK(Delete)
    MENU_LINK(DeleteChildren)
    MENU_LINK(DeleteAll)
    MENU_LINK(Recreate)
    MENU_LINK(ToggleImages)
    MENU_LINK(ToggleButtons)
    MENU_LINK(SetImageSize)
    MENU_LINK(CollapseAndReset)
    MENU_LINK(EnsureVisible)
    MENU_LINK(AddItem)
    MENU_LINK(InsertItem)
    MENU_LINK(IncIndent)
    MENU_LINK(DecIndent)
    MENU_LINK(IncSpacing)
    MENU_LINK(DecSpacing)
    MENU_LINK(ToggleIcon)
    MENU_LINK(SelectRoot)
#undef MENU_LINK

END_EVENT_TABLE()

#if USE_GENERIC_TREECTRL
BEGIN_EVENT_TABLE(MyTreeCtrl, wxGenericTreeCtrl)
#else
BEGIN_EVENT_TABLE(MyTreeCtrl, wxTreeCtrl)
#endif
    EVT_TREE_BEGIN_DRAG(TreeTest_Ctrl, MyTreeCtrl::OnBeginDrag)
    EVT_TREE_BEGIN_RDRAG(TreeTest_Ctrl, MyTreeCtrl::OnBeginRDrag)
    EVT_TREE_END_DRAG(TreeTest_Ctrl, MyTreeCtrl::OnEndDrag)
    EVT_TREE_BEGIN_LABEL_EDIT(TreeTest_Ctrl, MyTreeCtrl::OnBeginLabelEdit)
    EVT_TREE_END_LABEL_EDIT(TreeTest_Ctrl, MyTreeCtrl::OnEndLabelEdit)
    EVT_TREE_DELETE_ITEM(TreeTest_Ctrl, MyTreeCtrl::OnDeleteItem)
#if 0       // there are so many of those that logging them causes flicker
    EVT_TREE_GET_INFO(TreeTest_Ctrl, MyTreeCtrl::OnGetInfo)
#endif
    EVT_TREE_SET_INFO(TreeTest_Ctrl, MyTreeCtrl::OnSetInfo)
    EVT_TREE_ITEM_EXPANDED(TreeTest_Ctrl, MyTreeCtrl::OnItemExpanded)
    EVT_TREE_ITEM_EXPANDING(TreeTest_Ctrl, MyTreeCtrl::OnItemExpanding)
    EVT_TREE_ITEM_COLLAPSED(TreeTest_Ctrl, MyTreeCtrl::OnItemCollapsed)
    EVT_TREE_ITEM_COLLAPSING(TreeTest_Ctrl, MyTreeCtrl::OnItemCollapsing)

    EVT_TREE_SEL_CHANGED(TreeTest_Ctrl, MyTreeCtrl::OnSelChanged)
    EVT_TREE_SEL_CHANGING(TreeTest_Ctrl, MyTreeCtrl::OnSelChanging)
    EVT_TREE_KEY_DOWN(TreeTest_Ctrl, MyTreeCtrl::OnTreeKeyDown)
    EVT_TREE_ITEM_ACTIVATED(TreeTest_Ctrl, MyTreeCtrl::OnItemActivated)

    // so many differents ways to handle right mouse button clicks...
    EVT_CONTEXT_MENU(MyTreeCtrl::OnContextMenu)
    // EVT_TREE_ITEM_MENU is the preferred event for creating context menus
    // on a tree control, because it includes the point of the click or item,
    // meaning that no additional placement calculations are required.
    EVT_TREE_ITEM_MENU(TreeTest_Ctrl, MyTreeCtrl::OnItemMenu)
    EVT_TREE_ITEM_RIGHT_CLICK(TreeTest_Ctrl, MyTreeCtrl::OnItemRClick)

    EVT_RIGHT_DOWN(MyTreeCtrl::OnRMouseDown)
    EVT_RIGHT_UP(MyTreeCtrl::OnRMouseUp)
    EVT_RIGHT_DCLICK(MyTreeCtrl::OnRMouseDClick)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    // Create the main frame window
    MyFrame *frame = new MyFrame(wxT("wxTreeCtrl Test"), 50, 50, 450, 600);

    // Show the frame
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}


// My frame constructor
MyFrame::MyFrame(const wxString& title, int x, int y, int w, int h)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxPoint(x, y), wxSize(w, h)),
         m_treeCtrl(NULL)
#if wxUSE_LOG
         , m_textCtrl(NULL)
#endif // wxUSE_LOG
{
    // This reduces flicker effects - even better would be to define
    // OnEraseBackground to do nothing. When the tree control's scrollbars are
    // show or hidden, the frame is sent a background erase event.
    SetBackgroundColour(wxColour(255, 255, 255));

    // Give it an icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // Make a menubar
    wxMenu *file_menu = new wxMenu,
           *style_menu = new wxMenu,
           *tree_menu = new wxMenu,
           *item_menu = new wxMenu;

    file_menu->Append(TreeTest_About, wxT("&About..."));
    file_menu->AppendSeparator();
    file_menu->Append(TreeTest_Quit, wxT("E&xit\tAlt-X"));

    style_menu->AppendCheckItem(TreeTest_TogButtons, wxT("Toggle &normal buttons"));
    style_menu->AppendCheckItem(TreeTest_TogTwist, wxT("Toggle &twister buttons"));
    style_menu->AppendCheckItem(TreeTest_ToggleButtons, wxT("Toggle image &buttons"));
    style_menu->AppendSeparator();
    style_menu->AppendCheckItem(TreeTest_TogLines, wxT("Toggle &connecting lines"));
    style_menu->AppendCheckItem(TreeTest_TogRootLines, wxT("Toggle &lines at root"));
    style_menu->AppendCheckItem(TreeTest_TogHideRoot, wxT("Toggle &hidden root"));
    style_menu->AppendCheckItem(TreeTest_TogBorder, wxT("Toggle &item border"));
    style_menu->AppendCheckItem(TreeTest_TogFullHighlight, wxT("Toggle &full row highlight"));
    style_menu->AppendCheckItem(TreeTest_TogEdit, wxT("Toggle &edit mode"));
#ifndef NO_MULTIPLE_SELECTION
    style_menu->AppendCheckItem(TreeTest_ToggleSel, wxT("Toggle &selection mode"));
#endif // NO_MULTIPLE_SELECTION
    style_menu->AppendCheckItem(TreeTest_ToggleImages, wxT("Toggle show ima&ges"));
    style_menu->Append(TreeTest_SetImageSize, wxT("Set image si&ze..."));
    style_menu->AppendSeparator();
    style_menu->Append(TreeTest_SetFgColour, wxT("Set &foreground colour..."));
    style_menu->Append(TreeTest_SetBgColour, wxT("Set &background colour..."));
    style_menu->AppendSeparator();
    style_menu->Append(TreeTest_ResetStyle, wxT("&Reset to default\tF10"));

    tree_menu->Append(TreeTest_Recreate, wxT("&Recreate the tree"));
    tree_menu->Append(TreeTest_CollapseAndReset, wxT("C&ollapse and reset"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeTest_AddItem, wxT("Append a &new item"));
    tree_menu->Append(TreeTest_InsertItem, wxT("&Insert a new item"));
    tree_menu->Append(TreeTest_Delete, wxT("&Delete this item"));
    tree_menu->Append(TreeTest_DeleteChildren, wxT("Delete &children"));
    tree_menu->Append(TreeTest_DeleteAll, wxT("Delete &all items"));
    tree_menu->Append(TreeTest_SelectRoot, wxT("Select root item"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeTest_Count, wxT("Count children of current item"));
    tree_menu->Append(TreeTest_CountRec, wxT("Recursively count children of current item"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeTest_Sort, wxT("Sort children of current item"));
    tree_menu->Append(TreeTest_SortRev, wxT("Sort in reversed order"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeTest_EnsureVisible, wxT("Make the last item &visible"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeTest_IncIndent, wxT("Add 5 points to indentation\tAlt-I"));
    tree_menu->Append(TreeTest_DecIndent, wxT("Reduce indentation by 5 points\tAlt-R"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeTest_IncSpacing, wxT("Add 5 points to spacing\tCtrl-I"));
    tree_menu->Append(TreeTest_DecSpacing, wxT("Reduce spacing by 5 points\tCtrl-R"));

    item_menu->Append(TreeTest_Dump, wxT("&Dump item children"));
    item_menu->Append(TreeTest_Rename, wxT("&Rename item..."));

    item_menu->AppendSeparator();
    item_menu->Append(TreeTest_SetBold, wxT("Make item &bold"));
    item_menu->Append(TreeTest_ClearBold, wxT("Make item &not bold"));
    item_menu->AppendSeparator();
    item_menu->Append(TreeTest_ToggleIcon, wxT("Toggle the item's &icon"));

#ifndef NO_MULTIPLE_SELECTION
    item_menu->AppendSeparator();
    item_menu->Append(TreeTest_DumpSelected, wxT("Dump selected items\tAlt-D"));
    item_menu->Append(TreeTest_Select, wxT("Select current item\tAlt-S"));
    item_menu->Append(TreeTest_Unselect, wxT("Unselect everything\tAlt-U"));
#endif // NO_MULTIPLE_SELECTION

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, wxT("&File"));
    menu_bar->Append(style_menu, wxT("&Style"));
    menu_bar->Append(tree_menu, wxT("&Tree"));
    menu_bar->Append(item_menu, wxT("&Item"));
    SetMenuBar(menu_bar);
#endif // wxUSE_MENUS

    m_panel = new wxPanel(this);

#if wxUSE_LOG
    // create the controls
    m_textCtrl = new wxTextCtrl(m_panel, wxID_ANY, wxT(""),
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE | wxSUNKEN_BORDER);
#endif // wxUSE_LOG

    CreateTreeWithDefStyle();

    menu_bar->Check(TreeTest_ToggleImages, true);

#if wxUSE_STATUSBAR
    // create a status bar
    CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

#if wxUSE_LOG
#ifdef __WXMOTIF__
    // For some reason, we get a memcpy crash in wxLogStream::DoLogStream
    // on gcc/wxMotif, if we use wxLogTextCtl. Maybe it's just gcc?
    delete wxLog::SetActiveTarget(new wxLogStderr);
#else
    // set our text control as the log target
    wxLogTextCtrl *logWindow = new wxLogTextCtrl(m_textCtrl);
    delete wxLog::SetActiveTarget(logWindow);
#endif
#endif // wxUSE_LOG
}

MyFrame::~MyFrame()
{
#if wxUSE_LOG
    delete wxLog::SetActiveTarget(NULL);
#endif // wxUSE_LOG
}

void MyFrame::CreateTreeWithDefStyle()
{
    long style = wxTR_DEFAULT_STYLE |
#ifndef NO_VARIABLE_HEIGHT
                 wxTR_HAS_VARIABLE_ROW_HEIGHT |
#endif
                 wxTR_EDIT_LABELS;

    CreateTree(style | wxSUNKEN_BORDER);

    // as we don't know what wxTR_DEFAULT_STYLE could contain, test for
    // everything
    wxMenuBar *mbar = GetMenuBar();
    mbar->Check(TreeTest_TogButtons, (style & wxTR_HAS_BUTTONS) != 0);
    mbar->Check(TreeTest_TogButtons, (style & wxTR_TWIST_BUTTONS) != 0);
    mbar->Check(TreeTest_TogLines, (style & wxTR_NO_LINES) == 0);
    mbar->Check(TreeTest_TogRootLines, (style & wxTR_LINES_AT_ROOT) != 0);
    mbar->Check(TreeTest_TogHideRoot, (style & wxTR_HIDE_ROOT) != 0);
    mbar->Check(TreeTest_TogEdit, (style & wxTR_EDIT_LABELS) != 0);
    mbar->Check(TreeTest_TogBorder, (style & wxTR_ROW_LINES) != 0);
    mbar->Check(TreeTest_TogFullHighlight, (style & wxTR_FULL_ROW_HIGHLIGHT) != 0);
}

void MyFrame::CreateTree(long style)
{
    m_treeCtrl = new MyTreeCtrl(m_panel, TreeTest_Ctrl,
                                wxDefaultPosition, wxDefaultSize,
                                style);
    Resize();
}

void MyFrame::TogStyle(int id, long flag)
{
    long style = m_treeCtrl->GetWindowStyle() ^ flag;

    // most treectrl styles can't be changed on the fly using the native
    // control and the tree must be recreated
#ifndef __WXMSW__
    m_treeCtrl->SetWindowStyle(style);
#else // MSW
    delete m_treeCtrl;
    CreateTree(style);
#endif // !MSW/MSW

    GetMenuBar()->Check(id, (style & flag) != 0);
}

void MyFrame::OnIdle(wxIdleEvent& event)
{
#if wxUSE_STATUSBAR
    if ( m_treeCtrl )
    {
        wxTreeItemId idRoot = m_treeCtrl->GetRootItem();
        wxString status;
        if (idRoot.IsOk())
        {
            wxTreeItemId idLast = m_treeCtrl->GetLastChild(idRoot);
            status = wxString::Format(
                _T("Root/last item is %svisible/%svisible"),
                m_treeCtrl->IsVisible(idRoot) ? _T("") : _T("not "),
                idLast.IsOk() && m_treeCtrl->IsVisible(idLast)
                    ? _T("") : _T("not "));
        }
        else
            status = _T("No root item");

        SetStatusText(status, 1);
    }
#endif // wxUSE_STATUSBAR

    event.Skip();
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    if ( m_treeCtrl
#if wxUSE_LOG
                    && m_textCtrl
#endif // wxUSE_LOG
                                  )
    {
        Resize();
    }

    event.Skip();
}

void MyFrame::Resize()
{
    wxSize size = GetClientSize();
    m_treeCtrl->SetSize(0, 0, size.x, size.y
#if !wxUSE_LOG
                                            );
#else
                                            *2/3);
    m_textCtrl->SetSize(0, 2*size.y/3, size.x, size.y/3);
#endif
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("Tree test sample\n")
                 wxT("(c) Julian Smart 1997, Vadim Zeitlin 1998"),
                 wxT("About tree test"),
                 wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnRename(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    // old code - now we edit in place
#if 0
    static wxString s_text;
    s_text = wxGetTextFromUser(wxT("New name: "), wxT("Tree sample question"),
            s_text, this);
    if ( !s_text.empty() )
    {
        m_treeCtrl->SetItemText(item, s_text);
    }
#endif // 0

    // TODO demonstrate creating a custom edit control...
    (void)m_treeCtrl->EditLabel(item);
}

void MyFrame::OnCount(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    int i = m_treeCtrl->GetChildrenCount( item, false );

    wxLogMessage(wxT("%d children"), i);
}

void MyFrame::OnCountRec(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    int i = m_treeCtrl->GetChildrenCount( item );

    wxLogMessage(wxT("%d children"), i);
}

void MyFrame::DoSort(bool reverse)
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeCtrl->DoSortChildren(item, reverse);
}

void MyFrame::OnHighlight(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId id = m_treeCtrl->GetSelection();

    CHECK_ITEM( id );

    wxRect r;
    if ( !m_treeCtrl->GetBoundingRect(id, r, true /* text, not full row */) )
    {
        wxLogMessage(_T("Failed to get bounding item rect"));
        return;
    }

    wxClientDC dc(m_treeCtrl);
    dc.SetBrush(*wxRED);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(r);
    m_treeCtrl->Update();
}

void MyFrame::OnDump(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId root = m_treeCtrl->GetSelection();

    CHECK_ITEM( root );

    m_treeCtrl->GetItemsRecursively(root);
}

#ifndef NO_MULTIPLE_SELECTION

void MyFrame::OnToggleSel(wxCommandEvent& event)
{
    TogStyle(event.GetId(), wxTR_MULTIPLE);
}

void MyFrame::OnDumpSelected(wxCommandEvent& WXUNUSED(event))
{
    wxArrayTreeItemIds array;

    size_t count = m_treeCtrl->GetSelections(array);
    wxLogMessage(wxT("%u items selected"), unsigned(count));

    for ( size_t n = 0; n < count; n++ )
    {
        wxLogMessage(wxT("\t%s"), m_treeCtrl->GetItemText(array.Item(n)).c_str());
    }
}

void MyFrame::OnSelect(wxCommandEvent& WXUNUSED(event))
{
    m_treeCtrl->SelectItem(m_treeCtrl->GetSelection());
}

void MyFrame::OnSelectRoot(wxCommandEvent& WXUNUSED(event))
{
    m_treeCtrl->SelectItem(m_treeCtrl->GetRootItem());
}

void MyFrame::OnUnselect(wxCommandEvent& WXUNUSED(event))
{
    m_treeCtrl->UnselectAll();
}

#endif // NO_MULTIPLE_SELECTION

void MyFrame::DoSetBold(bool bold)
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeCtrl->SetItemBold(item, bold);
}

void MyFrame::OnDelete(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeCtrl->Delete(item);
}

void MyFrame::OnDeleteChildren(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeCtrl->DeleteChildren(item);
}

void MyFrame::OnDeleteAll(wxCommandEvent& WXUNUSED(event))
{
    m_treeCtrl->DeleteAllItems();
}

void MyFrame::OnRecreate(wxCommandEvent& event)
{
    OnDeleteAll(event);
    m_treeCtrl->AddTestItemsToTree(5, 2);
}

void MyFrame::OnSetImageSize(wxCommandEvent& WXUNUSED(event))
{
    int size = wxGetNumberFromUser(wxT("Enter the size for the images to use"),
                                    wxT("Size: "),
                                    wxT("TreeCtrl sample"),
                                    m_treeCtrl->ImageSize());
    if ( size == -1 )
        return;

    m_treeCtrl->CreateImageList(size);
    wxGetApp().SetShowImages(true);
}

void MyFrame::OnToggleImages(wxCommandEvent& WXUNUSED(event))
{
    if ( wxGetApp().ShowImages() )
    {
        m_treeCtrl->CreateImageList(-1);
        wxGetApp().SetShowImages(false);
    }
    else
    {
        m_treeCtrl->CreateImageList(0);
        wxGetApp().SetShowImages(true);
    }
}

void MyFrame::OnToggleButtons(wxCommandEvent& WXUNUSED(event))
{
#if USE_GENERIC_TREECTRL || !defined(__WXMSW__)
    if ( wxGetApp().ShowButtons() )
    {
        m_treeCtrl->CreateButtonsImageList(-1);
        wxGetApp().SetShowButtons(false);
    }
    else
    {
        m_treeCtrl->CreateButtonsImageList(15);
        wxGetApp().SetShowButtons(true);
    }
#endif
}

void MyFrame::OnCollapseAndReset(wxCommandEvent& WXUNUSED(event))
{
    m_treeCtrl->CollapseAndReset(m_treeCtrl->GetRootItem());
}

void MyFrame::OnEnsureVisible(wxCommandEvent& WXUNUSED(event))
{
    m_treeCtrl->DoEnsureVisible();
}

void MyFrame::OnInsertItem(wxCommandEvent& WXUNUSED(event))
{
    int image = wxGetApp().ShowImages() ? MyTreeCtrl::TreeCtrlIcon_File : -1;
    m_treeCtrl->InsertItem(m_treeCtrl->GetRootItem(), image, wxT("2nd item"));
}

void MyFrame::OnAddItem(wxCommandEvent& WXUNUSED(event))
{
    static int s_num = 0;

    wxString text;
    text.Printf(wxT("Item #%d"), ++s_num);

    m_treeCtrl->AppendItem(m_treeCtrl->GetRootItem(),
                           text /*,
                           MyTreeCtrl::TreeCtrlIcon_File */ );
}

void MyFrame::OnIncIndent(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = m_treeCtrl->GetIndent();
    if (indent < 100)
        m_treeCtrl->SetIndent( indent+5 );
}

void MyFrame::OnDecIndent(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = m_treeCtrl->GetIndent();
    if (indent > 10)
        m_treeCtrl->SetIndent( indent-5 );
}

void MyFrame::OnIncSpacing(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = m_treeCtrl->GetSpacing();
    if (indent < 100)
        m_treeCtrl->SetSpacing( indent+5 );
}

void MyFrame::OnDecSpacing(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = m_treeCtrl->GetSpacing();
    if (indent > 10)
        m_treeCtrl->SetSpacing( indent-5 );
}

void MyFrame::OnToggleIcon(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeCtrl->DoToggleIcon(item);
}

void MyFrame::OnSetFgColour(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, m_treeCtrl->GetForegroundColour());
    if ( col.Ok() )
        m_treeCtrl->SetForegroundColour(col);
}

void MyFrame::OnSetBgColour(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, m_treeCtrl->GetBackgroundColour());
    if ( col.Ok() )
        m_treeCtrl->SetBackgroundColour(col);
}
