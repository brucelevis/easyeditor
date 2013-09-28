#include "AbstractEditCMPT.h"

#include "common/tools.h"
#include "operator/AbstractEditOP.h"
#include "view/EditPanel.h"

namespace d2d
{

AbstractEditCMPT::AbstractEditCMPT(wxWindow* parent, const wxString& name, EditPanel* editPanel,
								   const wxString& childrenName/* = wxEmptyString*/)
	: wxScrolledWindow(parent)
	, m_editPanel(editPanel)
	, m_editOP(NULL)
	, m_name(name)
	, m_childrenName(childrenName)
	, m_childrenSizer(NULL)
{
}

AbstractEditCMPT::~AbstractEditCMPT()
{
	if (m_editOP)
		m_editOP->release();
 
 	for_each(m_children.begin(), m_children.end(), DeletePointerFunctor<AbstractEditCMPT>());
}

wxSizer* AbstractEditCMPT::initChildrenLayout()
{
	m_childrenSizer = new wxBoxSizer(wxVERTICAL);

	if (m_children.size() > 1)
	{
		wxArrayString choices;
		for (size_t i = 0, n = m_children.size(); i < n; ++i)
			choices.Add(m_children[i]->m_name);

		m_editChoice = new wxRadioBox(this, wxID_ANY, m_childrenName == wxEmptyString ? wxT("Operation") : m_childrenName, 
			wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
		Connect(m_editChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(AbstractEditCMPT::onChangeEditType));
		m_childrenSizer->Add(m_editChoice, 0);

		m_childrenSizer->AddSpacer(20);

		for (size_t i = 0, n = m_children.size(); i < n; ++i)
		{
			AbstractEditCMPT* child = m_children[i];
			child->SetSizer(child->initLayout());
			m_childrenSizer->Add(child);

			m_childrenSizer->Show(i + CMPT_SIZER_INDEX_OFFSET, i == 0);
		}
	}
	else if (m_children.size() == 1)
	{
		AbstractEditCMPT* child = m_children[0];
		child->SetSizer(child->initLayout());
		m_childrenSizer->Add(child);

		m_childrenSizer->Show((size_t)0, true);
	}

	if (!m_children.empty()) 
		m_children[0]->loadEditOP(this);

	return m_childrenSizer;
}

void AbstractEditCMPT::setChoice(size_t index)
{
	if (index >= 0 && index < m_children.size())
	{
		m_editChoice->SetSelection(index);
		onChangeEditType(wxCommandEvent());
	}
}

void AbstractEditCMPT::loadEditOP()
{
	m_editPanel->setEditOP(m_editOP);
	m_editPanel->Refresh();
}

void AbstractEditCMPT::loadEditOP(AbstractEditCMPT* cmpt)
{
	if (cmpt->m_editOP)
	{
		cmpt->m_editOP->clear();
		cmpt->m_editOP->release();
	}
	m_editOP->retain();
	cmpt->m_editOP = m_editOP;

	m_editPanel->setEditOP(m_editOP);
	m_editPanel->Refresh();
}

void AbstractEditCMPT::onChangeEditType(wxCommandEvent& event)
{
	int index = m_editChoice->GetSelection();

	for (size_t i = 0, n = m_children.size(); i < n; ++i)
	{
		m_childrenSizer->Show(i + CMPT_SIZER_INDEX_OFFSET, i == index);
		m_children[i]->show(i == index);

		m_children[i]->Layout();
	}
	Layout();

	if (index < m_children.size())
		m_children[index]->loadEditOP(this);
}

} // d2d