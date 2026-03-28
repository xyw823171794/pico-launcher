# Localization text keys (romBrowser views)

Scanned `SetText(u"...")` / `SetText("...")` call sites under `arm9/source/romBrowser/views/` and mapped hardcoded UI text to keys:

| Key | English | 中文 | Original call site |
| --- | --- | --- | --- |
| `ui.display_settings.title` | Display Settings | 显示设置 | `DisplaySettingsBottomSheetView::_titleLabel.SetText(...)` |
| `ui.display_settings.layout` | Layout | 布局 | `DisplaySettingsBottomSheetView::_layoutLabel.SetText(...)` |
| `ui.display_settings.sorting` | Sorting | 排序 | `DisplaySettingsBottomSheetView::_sortingLabel.SetText(...)` |
| `ui.display_settings.filters` | Filters | 筛选 | Commented `DisplaySettingsBottomSheetView::_filtersLabel.SetText(...)` |
| `ui.game_details.cheats` | Cheats | 金手指 | `NdsGameDetailsBottomSheetView::_cheatsChip.SetText(...)` |
| `ui.game_details.favorite` | Favorite | 收藏 | `NdsGameDetailsBottomSheetView::_favoriteChip.SetText(...)` |
| `ui.cheats.title` | Cheats | 金手指 | `CheatsBottomSheetView::_titleLabel.SetText(...)` |
| `ui.cheats.empty` | No cheats found. | 未找到金手指。 | `CheatsBottomSheetView::_secondaryLabel.SetText(...)` |
| `ui.app_bar.back` | Back | 返回 | AppBar 按钮标签资源 |
| `ui.app_bar.display_settings` | Display Settings | 显示设置 | AppBar 按钮标签资源 |

Notes:
- `SetText("")` empty placeholders are not localized.
- Dynamic strings from ROM/cheat data are not localized by key.
