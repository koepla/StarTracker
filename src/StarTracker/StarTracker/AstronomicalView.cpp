#include "AstronomicalView.hpp"

namespace StarTracker {
	
	AstronomicalView::AstronomicalView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {

	}

	void AstronomicalView::OnInit() noexcept {

		bodyLibrary = Core::AssetDataBase::LoadBodyLibrary("CelestialBodies.json");
		observer = [&]() -> Utils::LocationService::Location {

			try {

				return Utils::LocationService::GeoLocation::Get();
			}
			catch (const std::exception&) {

				return {};
			}
		}();
	}

	void AstronomicalView::OnUpdate(float deltaTime) noexcept {

		if (ImGui::Begin("Astronomical Calculations", nullptr, ImGuiWindowFlags_NoScrollbar)) {

			const auto flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
			if (ImGui::BeginTable("##idChildAstronomicalAlignmentTable", 2, flags)) {

				const auto totalWidth = ImGui::GetContentRegionAvail().x;

				ImGui::TableSetupColumn("##idColumnPositionsPanel", ImGuiTableColumnFlags_WidthStretch, 0.25f * totalWidth);
				ImGui::TableSetupColumn("##idColumnEphemerisPanel", ImGuiTableColumnFlags_WidthStretch, 0.25f * totalWidth);
			
				ImGui::TableNextRow();
				{
					ImGui::TableSetColumnIndex(0);
					{
						drawPositions();
					}
					ImGui::TableSetColumnIndex(1);
					{
						drawEphemeris();
					}
				}

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}

	void AstronomicalView::OnDestroy() noexcept {


	}

	void AstronomicalView::drawPositions() noexcept {

		const auto& style = ImGui::GetStyle();
		const auto fontSize = ImGui::GetFontSize();
		const auto itemSpacing = style.ItemSpacing;
		const auto itemInnerSpacing = style.ItemInnerSpacing;
		const auto& baseTextColor = style.Colors[ImGuiCol_Text];

		UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };

		if (ImGui::BeginChild("idChildAstronomicalPositions", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

			if (ImGui::BeginChild("idChildAstronomicalPositionsHeader", { ImGui::GetContentRegionAvail().x, fontSize }, false, ImGuiWindowFlags_NoScrollbar)) {

				UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
				UI::Text::Draw("Positions", UI::Font::Medium, fontSize, baseTextColor);
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}

	void AstronomicalView::drawEphemeris() noexcept {

		const auto& style = ImGui::GetStyle();
		const auto fontSize = ImGui::GetFontSize();
		const auto itemSpacing = style.ItemSpacing;
		const auto itemInnerSpacing = style.ItemInnerSpacing;
		const auto& baseTextColor = style.Colors[ImGuiCol_Text];

		UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };

		if (ImGui::BeginChild("idChildAstronomicalEphemeris", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

			if (ImGui::BeginChild("idChildFrameBufferPanelHeader", { ImGui::GetContentRegionAvail().x, fontSize }, false, ImGuiWindowFlags_NoScrollbar)) {

				UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
				UI::Text::Draw("Ephemeris", UI::Font::Medium, fontSize, baseTextColor);
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}

	void AstronomicalView::drawGraph() noexcept {

	}
}