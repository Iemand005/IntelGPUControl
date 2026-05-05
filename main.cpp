
#include <ze_api.h>

#include <wx/wx.h>

// int main() {
//   zeInit(ZE_INIT_FLAG_GPU_ONLY);


// }

class IntelGPUControlApp : public wxApp {
public:
    virtual bool OnInit() override {
        wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(450, 340));
        
        wxPanel *panel = new wxPanel(frame, wxID_ANY);

         wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        wxSlider *slider = new wxSlider(panel, wxID_ANY, 50, 0, 100, 
                                        wxDefaultPosition, wxDefaultSize, 
                                        wxSL_HORIZONTAL | wxSL_LABELS);
        sizer->Add(slider, 0, wxALL | wxEXPAND, 10);

        wxButton *button = new wxButton(panel, wxID_ANY, "Set");
        sizer->Add(button, 0, wxALL | wxCENTER, 10);

        panel->SetSizer(sizer);

        button->Bind(wxEVT_BUTTON, [slider](wxCommandEvent&) {
            wxMessageBox(wxString::Format("%d", slider->GetValue()), 
                        "Info", wxOK | wxICON_INFORMATION);
        });

        frame->Show(true);
        
        return true;
    }
};

wxIMPLEMENT_APP(IntelGPUControlApp);