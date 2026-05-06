
#include <stdexcept>

#include <ze_api.h>
#include <zes_api.h>

#include <wx/wx.h>

class IntelFrequencyController {
    ze_device_handle_t hDevice = nullptr;
    std::vector<zes_freq_handle_t> phFrequency;

    void LoadFrequencyRanges() {
        uint32_t freqDomainCount = 0;
        ze_result_t result = zesDeviceEnumFrequencyDomains(hDevice, &freqDomainCount, nullptr);

        if (result != ZE_RESULT_SUCCESS || freqDomainCount == 0)
            throw std::runtime_error("No frequency domains found.");

        phFrequency = std::vector<zes_freq_handle_t>(freqDomainCount);

        result = zesDeviceEnumFrequencyDomains(hDevice, &freqDomainCount, phFrequency.data());

        if (result != ZE_RESULT_SUCCESS)
            throw std::runtime_error("Failed to enumerate frequency domains.");
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
        for (auto hFrequency : phFrequency)
            if (zesFrequencyGetRange(hFrequency, &limits) == ZE_RESULT_SUCCESS)
                return limits;
        throw std::runtime_error("Failed to get frequency limits.");
    }

    void SetFrequencyRange(double min, double max) {
        zes_freq_range_t limits = { min, max };
        for (auto hFrequency : phFrequency)
            if (zesFrequencySetRange(hFrequency, &limits) == ZE_RESULT_SUCCESS)
                return;
        throw std::runtime_error("Failed to set frequency limits.");
    }
};

class IntelGPUControlApp : public wxApp, public IntelFrequencyController {

    void ShowError(std::runtime_error error) {
        wxMessageBox(error.what(), "Error", wxCLOSE | wxICON_ERROR);
    }

public:
    virtual bool OnInit() override {

        try {
            Init();
        } catch (std::runtime_error error) { ShowError(error); }

        wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(450, 340));
        
        wxPanel *panel = new wxPanel(frame, wxID_ANY);

        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        wxSlider *minSlider = new wxSlider(panel, wxID_ANY, 0, 0, 100,  wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
        sizer->Add(minSlider, 0, wxALL | wxEXPAND, 10);

        wxSlider *maxSlider = new wxSlider(panel, wxID_ANY, 100, 0, 100,  wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
        sizer->Add(maxSlider, 0, wxALL | wxEXPAND, 10);

        wxButton *button = new wxButton(panel, wxID_ANY, "Set");
        sizer->Add(button, 0, wxALL | wxCENTER, 10);

        panel->SetSizer(sizer);

        button->Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
            wxMessageBox(wxString::Format("%d", minSlider->GetValue()), "Info", wxOK | wxICON_INFORMATION);

            double min = ((double)minSlider->GetValue()) / (double)minSlider->GetMax();
            double max = ((double)maxSlider->GetValue()) / (double)maxSlider->GetMax();

            try {
                SetFrequencyRange(min, max);
            } catch (std::runtime_error error) { ShowError(error); }
        });

        frame->Show(true);
        
        return true;
    }
};

wxIMPLEMENT_APP(IntelGPUControlApp);