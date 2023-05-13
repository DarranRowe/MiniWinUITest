#include "pch.h"
#include "XamlApplication.h"
#include "XamlApplication.g.cpp"

namespace winrt
{
    namespace wf = winrt::Windows::Foundation;
    namespace wfc = winrt::Windows::Foundation::Collections;

    namespace wuxi = winrt::Windows::UI::Xaml::Interop;
    namespace muxm = winrt::Microsoft::UI::Xaml::Markup;
}

namespace winrt::MiniWinUITest::implementation
{
    XamlApplication::XamlApplication(std::initializer_list<muxm::IXamlMetadataProvider> providers) : m_providers(winrt::single_threaded_vector<muxm::IXamlMetadataProvider>())
    {
        for (auto &&provider : providers)
        {
            m_providers.Append(provider);
        }
    }
    wfc::IVector<muxm::IXamlMetadataProvider> XamlApplication::Providers()
    {
        return m_providers;
    }
    muxm::IXamlType XamlApplication::GetXamlType(wuxi::TypeName const& type)
    {
        for (const auto &provider : m_providers)
        {
            const auto xaml_type = provider.GetXamlType(type);
            if (xaml_type)
            {
                return xaml_type;
            }
        }

        return nullptr;
    }
    muxm::IXamlType XamlApplication::GetXamlType(hstring const& fullName)
    {
        for (const auto &provider : m_providers)
        {
            const auto xaml_type = provider.GetXamlType(fullName);
            if (xaml_type)
            {
                return xaml_type;
            }
        }

        return nullptr;
    }
    com_array<muxm::XmlnsDefinition> XamlApplication::GetXmlnsDefinitions()
    {
        std::vector<muxm::XmlnsDefinition> definitions;

        for (const auto &provider : m_providers)
        {
            auto defs = provider.GetXmlnsDefinitions();
            for (const auto &def : defs)
            {
                definitions.emplace_back(def);
            }
        }

        return winrt::com_array<muxm::XmlnsDefinition>(std::move(definitions));
    }

    void XamlApplication::OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const &)
    {
        auto resources = winrt::Microsoft::UI::Xaml::Controls::XamlControlsResources();
        Resources().MergedDictionaries().Append(resources);

        window = winrt::Microsoft::UI::Xaml::Window();

        auto c_base = winrt::Microsoft::UI::Xaml::Controls::StackPanel();
        c_base.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
        c_base.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
        c_base.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal);
        auto c_tb = winrt::Microsoft::UI::Xaml::Controls::TextBlock();
        c_tb.Text(L"MinWinUITest");
        c_base.Children().Append(c_tb);
        window.Content(c_base);

        window.Activate();
    }
}
