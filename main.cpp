
#include <ze_api.h>

#include <wx/wx.h>

// int main() {
//   zeInit(ZE_INIT_FLAG_GPU_ONLY);


// }

class IntelGPUControlApp : public wxApp {
public:
    virtual bool OnInit() override {

        zeInit(ZE_INIT_FLAG_GPU_ONLY);

        uint32_t driverCount = 0;
        zeDriverGet(&driverCount, nullptr);

        std::vector<ze_driver_handle_t> allDrivers(driverCount);
        zeDriverGet(&driverCount, allDrivers.data());

        
        
            // uint driverCount = 0;
            // result = LevelZeroInterop.GetDrivers(ref driverCount, null);
            // if (result != 0 || driverCount == 0)
            //     throw new Exception("No drivers found.");

            // _drivers = new DriverHandle[driverCount];
            // result = LevelZeroInterop.GetDrivers(ref driverCount, _drivers);
            // if (result != 0)
            //     throw new Exception("Failed to get drivers.");

            // uint deviceCount = 0;
            // result = LevelZeroInterop.GetDevices(_drivers[0], ref deviceCount);
            // if (result != 0 || deviceCount == 0)
            //     throw new Exception("No devices found.");

            // _devices = new DeviceHandle[deviceCount];
            // result = LevelZeroInterop.GetDevices(_drivers[0], ref deviceCount, _devices);
            // if (result != 0)
            //     throw new Exception("Failed to get devices.");


            // uint freqDomainCount = 0;
            // result = LevelZeroInterop.GetDeviceFrequencies(_devices[0], ref freqDomainCount);
            // if (result != 0 || freqDomainCount == 0)
            //     throw new Exception("No frequency domains found.");
            // _freqHandles = new FrequencyHandle[freqDomainCount];
            // result = LevelZeroInterop.GetDeviceFrequencies(_devices[0], ref freqDomainCount, _freqHandles);
            // if (result != 0)
            //     throw new Exception("Failed to enumerate frequency domains.");

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