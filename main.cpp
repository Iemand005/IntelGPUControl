
#include <ze_api.h>

#include <wx/wx.h>

// int main() {
//   zeInit(ZE_INIT_FLAG_GPU_ONLY);


// }

class IntelGPUControlApp : public wxApp {
public:
    virtual bool OnInit() override {
        wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(450, 340));
        
        frame->Show(true);
        
        return true;
    }
};

wxIMPLEMENT_APP(IntelGPUControlApp);