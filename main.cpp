
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

    zes_freq_range_t GetFrequencyRange() {
        zes_freq_range_t limits;
        if (zesFrequencyGetRange(hFrequency, &limits) == ZE_RESULT_SUCCESS)
            return limits;
        throw std::runtime_error("Failed to get frequency limits.");
    }

    void SetFrequencyRange(zes_freq_range_t limits) {
        if (zesFrequencySetRange(hFrequency, &limits) == ZE_RESULT_SUCCESS)
            return;
        throw std::runtime_error("Failed to set frequency limits.");
    }
};

class IntelGPUControlApp : public wxApp, public IntelFrequencyController {

    wxPanel *panel;
    wxBoxSizer *sizer;
    wxSlider *minSlider;
    wxSlider *maxSlider;

    void ShowError(std::runtime_error error) {
        wxMessageBox(error.what(), "Error", wxCLOSE | wxICON_ERROR);
    }

    double GetValue(wxSlider* s) { return static_cast<double>(s->GetValue()); };

public:
    virtual bool OnInit() override {
        try {
            Init();

            auto range = GetFrequencyRange();

            wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Intel GPU Control", wxDefaultPosition, wxSize(450, 340));
            
            panel = new wxPanel(frame, wxID_ANY);
            sizer = new wxBoxSizer(wxVERTICAL);

            minSlider = new wxSlider(panel, wxID_ANY, range.min, range.min, range.max,  wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
            sizer->Add(minSlider, 0, wxALL | wxEXPAND, 10);

            maxSlider = new wxSlider(panel, wxID_ANY, range.max, range.min, range.max,  wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
            sizer->Add(maxSlider, 0, wxALL | wxEXPAND, 10);

            wxButton *button = new wxButton(panel, wxID_ANY, "Set");
            sizer->Add(button, 0, wxALL | wxCENTER, 10);

            panel->SetSizer(sizer);

            button->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
                wxMessageBox(wxString::Format("%d", minSlider->GetValue()), "Info", wxOK | wxICON_INFORMATION);

                try {
                    SetFrequencyRange({ GetValue(minSlider), GetValue(maxSlider) });
                } catch (std::runtime_error error) { ShowError(error); }
            });

            // maxSlider->Bind(wxEVT_CHANGGE7)

            frame->Show(true);
            
            return true;
        } catch (std::runtime_error error) { ShowError(error); }

        return false;
    }
};

wxIMPLEMENT_APP(IntelGPUControlApp);