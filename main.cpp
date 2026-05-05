
#include <ze_api.h>
#include <zes_api.h>

#include <wx/wx.h>

// int main() {
//   zeInit(ZE_INIT_FLAG_GPU_ONLY);


// }

class IntelFrequencyController {
    ze_driver_handle_t hDriver = nullptr;
    ze_device_handle_t hDevice = nullptr;

};

class IntelGPUControlApp : public wxApp, public IntelFrequencyController {
public:
    virtual bool OnInit() override {

        zeInit(ZE_INIT_FLAG_GPU_ONLY);

        uint32_t driverCount = 0;
        zeDriverGet(&driverCount, nullptr);

        std::vector<ze_driver_handle_t> allDrivers(driverCount);
        zeDriverGet(&driverCount, allDrivers.data());

        

        for (auto& driver : allDrivers) {
            uint32_t deviceCount = 0;
            zeDeviceGet(driver, &deviceCount, nullptr);
            
            std::vector<ze_device_handle_t> allDevices(deviceCount);
            zeDeviceGet(driver, &deviceCount, allDevices.data());

            for (auto& device : allDevices) {
                ze_device_properties_t device_properties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
                zeDeviceGetProperties(device, &device_properties);
                
                if (ZE_DEVICE_TYPE_GPU == device_properties.type) {
                    hDriver = driver;
                    hDevice = device;
                    break;
                }
            }
            if (hDevice) break;
        }

        if (!hDevice) {
            wxMessageBox("Couldn't find Intel GPU.", "Info", wxCLOSE | wxICON_ERROR);
            return false;
        }

        uint32_t freqDomainCount;
        zesDeviceEnumFrequencyDomains(hDevice, &freqDomainCount, nullptr);


        std::vector<zes_freq_handle_t> hFrequency(freqDomainCount);
        zesDeviceEnumFrequencyDomains(hDevice, &freqDomainCount, hFrequency.data());

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