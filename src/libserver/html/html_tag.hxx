/*-
 * Copyright 2021 Vsevolod Stakhov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RSPAMD_HTML_TAG_HXX
#define RSPAMD_HTML_TAG_HXX
#pragma once

#include <utility>
#include <string_view>
#include <variant>
#include <vector>
#include <optional>

namespace rspamd::html {

enum class html_component_type : std::uint8_t {
	RSPAMD_HTML_COMPONENT_NAME = 0,
	RSPAMD_HTML_COMPONENT_HREF,
	RSPAMD_HTML_COMPONENT_COLOR,
	RSPAMD_HTML_COMPONENT_BGCOLOR,
	RSPAMD_HTML_COMPONENT_STYLE,
	RSPAMD_HTML_COMPONENT_CLASS,
	RSPAMD_HTML_COMPONENT_WIDTH,
	RSPAMD_HTML_COMPONENT_HEIGHT,
	RSPAMD_HTML_COMPONENT_SIZE,
	RSPAMD_HTML_COMPONENT_REL,
	RSPAMD_HTML_COMPONENT_ALT,
};

using html_tag_extra_t = std::variant<std::monostate, struct rspamd_url *, struct html_image *>;
struct html_tag_component {
	html_component_type type;
	std::string_view value;

	html_tag_component(html_component_type type, std::string_view value)
		: type(type), value(value) {}
};

struct html_tag {
	int id = -1;
	unsigned int flags = 0;
	mutable unsigned int content_length = 0; /* Allow content length propagation */
	unsigned int content_offset = 0;

	std::string_view name;
	std::vector<html_tag_component> parameters;

	html_tag_extra_t extra;
	struct html_block *block = nullptr; /* TODO: temporary, must be handled by css */
	std::vector<struct html_tag *> children;
	struct html_tag *parent;

	auto find_component(html_component_type what) const -> std::optional<std::string_view>
	{
		for (const auto &comp : parameters) {
			if (comp.type == what) {
				return comp.value;
			}
		}

		return std::nullopt;
	}
};

}

#endif //RSPAMD_HTML_TAG_HXX
