
#include <stdexcept>

#include <ze_api.h>
#include <zes_api.h>

#include <wx/wx.h>

class IntelFrequencyController {
    ze_device_handle_t hDevice = nullptr;
    zes_freq_handle_t hFrequency;

    void LoadFrequencyRanges() {
        uint32_t count = 0;
        if (zesDeviceEnumFrequencyDomains(hDevice, &count, nullptr) != ZE_RESULT_SUCCESS || count <= 0)
            throw std::runtime_error("No frequency domains found.");

        std::vector<zes_freq_handle_t> phFrequency(count);
        if (zesDeviceEnumFrequencyDomains(hDevice, &count, phFrequency.data()) != ZE_RESULT_SUCCESS)
            throw std::runtime_error("Failed to enumerate frequency domains.");
        
        hFrequency = phFrequency.front();
    }

public:
    void Init() {
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

            ze_device_properties_t device_properties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
            for (auto& device : allDevices) {
                zeDeviceGetProperties(device, &device_properties);
                
                if (device_properties.type != ZE_DEVICE_TYPE_GPU)
                    continue;
                
                hDevice = device;
            }

            if (!hDevice && allDevices.size() > 0)
                hDevice = allDevices[0];
        }
        
        if (!hDevice)
            throw std::runtime_error("Couldn't find Intel GPU.");
        
        LoadFrequencyRanges();
    }

    zes_freq_range_t GetSupportedFrequencyRange() {
        zes_freq_properties_t Properties;
        zesFrequencyGetProperties(hFrequency, &Properties);
        
        // if (!Properties.canControl)
        //     throw std::runtime_error("This GPU's frequency cannot be controlled");
        // Properties.
        return { Properties.min, Properties.max };
    }

    // public void GetSupportedFrequencyRange(out double min, out double max)
    //     {
    //         foreach (var handle in _freqHandles)
    //         {
    //             FrequencyProperties properties = new FrequencyProperties();
    //             int result = LevelZeroInterop.GetFrequencyProperties(handle, ref properties);
    //             if (result != 0)
    //                 throw new Exception("Failed to get frequency properties.");
    //             min = properties.Min;
    //             max = properties.Max;
    //             return;
    //         }
    //         throw new Exception("Frequency domain not found.");
    //     }

    zes_freq_range_t GetFrequencyRange() {
        zes_freq_range_t limits;
        if (zesFrequencyGetRange(hFrequency, &limits) == ZE_RESULT_SUCCESS)
            return limits;
        throw std::runtime_error("Failed to get frequency limits.");
    }

    void SetFrequencyRange(zes_freq_range_t limits) {
        if (zesFrequencySetRange(hFrequency, &limits) != ZE_RESULT_SUCCESS)
            throw std::runtime_error("Failed to set frequency limits. Setting requires admin rights!");
    }

    void ResetFrequencyRange() {
        SetFrequencyRange({ -1, -1 });
    }

    void ExtendFrequencyRange() {
        SetFrequencyRange({ 0, 0 });
    }
};

class IntelGPUControlApp : public wxApp, public IntelFrequencyController {

    void ShowError(std::runtime_error error) { wxMessageBox(error.what(), "Error", wxCLOSE | wxICON_ERROR); }

    double GetValue(wxSlider* s) { return static_cast<double>(s->GetValue()); };

public:
    virtual bool OnInit() override {
        try {
            Init();

            auto range = GetFrequencyRange();
            auto supportedRange = GetSupportedFrequencyRange();

            wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Intel GPU Control", wxDefaultPosition, wxSize(450, 340));
            
            wxPanel *panel = new wxPanel(frame, wxID_ANY);
            wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

            wxSlider *minSlider = new wxSlider(panel, wxID_ANY, range.min, supportedRange.min, supportedRange.max,  wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
            wxSlider *maxSlider = new wxSlider(panel, wxID_ANY, range.max, supportedRange.min, supportedRange.max,  wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
            
            sizer->Add(minSlider, 0, wxALL | wxEXPAND, 10);
            sizer->Add(maxSlider, 0, wxALL | wxEXPAND, 10);

            panel->SetSizer(sizer);

            auto updateSliders = [=]() {
                auto range = GetFrequencyRange();
                minSlider->SetValue(range.min);
                maxSlider->SetValue(range.max);
            };

            auto apply = [=]() { 
                try {
                    SetFrequencyRange({ GetValue(minSlider), GetValue(maxSlider) });
                } catch (std::runtime_error error) {
                    ShowError(error);
                    updateSliders();
                }
            };

            minSlider->Bind(wxEVT_SLIDER, apply);
            maxSlider->Bind(wxEVT_SLIDER, apply);


            wxButton *unlockButton = new wxButton(panel, wxID_ANY, "Unlock");
            wxButton *resetButton = new wxButton(panel, wxID_ANY, "Reset");

            sizer->Add(unlockButton);
            sizer->Add(resetButton);

            unlockButton->Bind(wxEVT_BUTTON, [=]() {
                ExtendFrequencyRange();
                updateSliders();
            });

            resetButton->Bind(wxEVT_BUTTON, [=]() {
                ResetFrequencyRange();
                updateSliders();
            });

            frame->Show(true);
            
            return true;
        } catch (std::runtime_error error) { ShowError(error); }

        return false;
    }
};

wxIMPLEMENT_APP(IntelGPUControlApp);