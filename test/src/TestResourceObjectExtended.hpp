#ifndef C_TEST_RESOURCE_OBJECT_EXTENDED_HPP
#define C_TEST_RESOURCE_OBJECT_EXTENDED_HPP

#include "TestResourceObject.hpp"

struct NestedObjectExtended {
	std::string nestedTestExtended = "nestedTestExtended";
};

class TestResourceObjectExtended : public TestResourceObject {
	GENERATE_RESOURCEABLE_EXTEND(TestResourceObjectExtended,
		NestedObjectExtended nestedExtended;
	, TestResourceObject)
public:
	TestResourceObjectExtended();
	~TestResourceObjectExtended();

	static auto deserialize(const std::vector<std::byte>& serialized) -> TestResourceObjectExtended*;
	[[nodiscard]] auto serialize() const -> std::vector<std::byte>;
};

#endif