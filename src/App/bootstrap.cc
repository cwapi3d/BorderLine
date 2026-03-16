//
// Created by MichaelBrunner on 09/12/2025.
//

#include <cwapi3d/CwAPI3D.h>
#include <cwapi3d/ICwAPI3DControllerFactory.h>
#include <cwapi3d/utility/StringConversion.h>

#include <QApplication>
#include <memory>

#include "AttributeControl.hh"
#include "EdgeOffsetConfig.hh"
#include "ElementControl.hh"
#include "GeometryControl.hh"
#include "ShopDrawingControl.hh"
#include "TranslationManager.hh"
#include "UtilityControl.hh"
#include "VisualizationControl.hh"
#include "cjtcedgesdialog.h"
#include "config/ConfigurationAdapter.hh"
#include "ports/IConfigurationPort.hh"
#include "services/ColorApplicationService.hh"
#include "services/EdgeImageService.hh"
#include "services/EdgeProfileWorkflowService.hh"
#include "services/ShopDrawingExportService.hh"
#include "services/TriangleSymbolService.hh"
#include "strategies/SortingStrategies.hh"
#include "strategies/SortingStrategyFactory.hh"
#include "strategies/StandardEdgeOffsetStrategy.hh"
#include "strategies/StandardTriangleStrategy.hh"
#include "viewmodels/EdgeImageViewModel.hh"

namespace {

borderline::ui::TranslationManager::Language toLanguage(const std::wstring &language) {
    if (std::ranges::equal(language, L"de")) {
        return borderline::ui::TranslationManager::Language::German;
    }
    if (std::ranges::equal(language, L"fr")) {
        return borderline::ui::TranslationManager::Language::French;
    }
    if (std::ranges::equal(language, L"it")) {
        return borderline::ui::TranslationManager::Language::Italian;
    }
    return borderline::ui::TranslationManager::Language::English;
}
}  // namespace

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *factory) {
    std::cout << "Hello from BorderLine!" << std::endl;

    using namespace borderline;

    auto elementControlPort = std::make_shared<adapter::cadwork::ElementControl>(factory);
    auto attributeControlPort = std::make_shared<adapter::cadwork::AttributeControl>(factory);
    auto geometryControlPort = std::make_shared<adapter::cadwork::GeometryControl>(factory);
    auto shopDrawingControlPort = std::make_shared<adapter::cadwork::ShopDrawingControl>(factory);
    auto elementFilterStrategy = std::make_shared<adapter::cadwork::ElementFilters>(factory);
    auto utilityControlPort = std::make_shared<adapter::cadwork::UtilityControl>(factory);
    auto visualizationControlPort = std::make_shared<adapter::cadwork::VisualizationControl>(factory);

    const std::string pluginPath = CwAPI3D::Utility::ToUtf8(factory->getUtilityController()->getPluginPath()->data());

    // Load edge offset configuration from file (DI via Port)
    const auto configAdapter
        = std::make_shared<adapter::ConfigurationAdapter>(QString::fromStdString(pluginPath) + "/config");
    const edgeprofile::EdgeOffsetConfig offsetConfig = configAdapter->loadEdgeOffsetConfig();

    auto triangleStrategy = std::make_shared<adapter::cadwork::StandardTriangleStrategy>();
    auto triangleSymbolService
        = std::make_shared<edgeprofile::TriangleSymbolService>(triangleStrategy, elementControlPort);

    auto offsetStrategy = std::make_shared<adapter::cadwork::StandardEdgeOffsetStrategy>();

    auto edgeImageService = std::make_shared<edgeprofile::EdgeImageService>(
        elementControlPort, geometryControlPort, attributeControlPort, elementFilterStrategy, triangleSymbolService,
        offsetStrategy, offsetConfig);

    auto exportService = std::make_shared<edgeprofile::services::ShopDrawingExportService>(shopDrawingControlPort,
                                                                                           attributeControlPort);

    auto colorApplicationService = std::make_shared<edgeprofile::services::ColorApplicationService>(
        utilityControlPort, visualizationControlPort);

    auto sortingStrategyFactory = std::make_shared<adapter::cadwork::SortingStrategyFactory>();

    auto elementSelection = [elementControlPort, elementFilterStrategy]() {
        return elementControlPort->getActiveElementIDsFilter(
            [elementFilterStrategy](const edgeprofile::ElementID id) { return elementFilterStrategy->isPanel(id); });
    };

    auto workflowService = std::make_shared<edgeprofile::services::EdgeProfileWorkflowService>(
        edgeImageService, exportService, colorApplicationService, attributeControlPort, sortingStrategyFactory,
        std::move(elementSelection));

    // defines how the edges are connected
    const edgeprofile::ProfileConfig config{
        .mBottom
        = {.mExistenceAttributeId = edgeprofile::AttributeIdx{505}, .mTypeAttributeId = edgeprofile::AttributeIdx{508}},
        .mLeft
        = {.mExistenceAttributeId = edgeprofile::AttributeIdx{509}, .mTypeAttributeId = edgeprofile::AttributeIdx{512}},
        .mTop
        = {.mExistenceAttributeId = edgeprofile::AttributeIdx{513}, .mTypeAttributeId = edgeprofile::AttributeIdx{516}},
        .mRight = {.mExistenceAttributeId = edgeprofile::AttributeIdx{517},
                   .mTypeAttributeId = edgeprofile::AttributeIdx{520}}};

    const auto translationManager
        = std::make_unique<ui::TranslationManager>(QString::fromStdString(pluginPath) + "/translations");

    std::ignore = translationManager->loadLanguage(toLanguage(factory->getUtilityController()->getLanguage()->data()));

    const auto viewModel = std::make_shared<ui::viewmodels::EdgeImageViewModel>(workflowService, config);

    auto *const hwndCadwork = factory->getUtilityController()->get3DHWND();
    auto *const mainWindowCadwork3d = QWidget::find(reinterpret_cast<WId>(hwndCadwork));
    auto retrieveUserColor
        = [utilityControlPort](const edgeprofile::Number colorNr) { return utilityControlPort->getUserColor(colorNr); };
    auto rgbColorExtractor = [visualizationControlPort](const edgeprofile::Number colorNr) {
        const auto [r, g, b] = visualizationControlPort->getRgbColor(colorNr);
        return std::tuple{r, g, b};
    };
    // TODO: Retrieve actual plugin path from factory or environment
    auto *const dialog
        = new CJtcEdgesDialog(viewModel, pluginPath, retrieveUserColor, rgbColorExtractor, mainWindowCadwork3d);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();

    return false;
}
