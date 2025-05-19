#include "VXamlParser.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

// ----------------------------------------
// XamlElement Implementation
// ----------------------------------------
XamlElement::XamlElement() : gridRow(0), gridColumn(0) {
    setName("XamlElement");
}

XamlElement::~XamlElement() = default;

void XamlElement::setName(const std::string& name) {
    elementName = name;
}

std::string XamlElement::getName() const {
    return elementName;
}

void XamlElement::setMargin(const std::string& margin) {
    this->margin = margin;
}

void XamlElement::setHorizontalAlignment(const std::string& alignment) {
    horizontalAlignment = alignment;
}

void XamlElement::setVerticalAlignment(const std::string& alignment) {
    verticalAlignment = alignment;
}

void XamlElement::setGridRow(int row) {
    gridRow = row;
}

void XamlElement::setGridColumn(int column) {
    gridColumn = column;
}

// ----------------------------------------
// XamlWindow Implementation
// ----------------------------------------
XamlWindow::XamlWindow() : width(800), height(600) {
    setName("XamlWindow");
}

XamlWindow::~XamlWindow() = default;

void XamlWindow::setTitle(const std::string& title) {
    this->title = title;
}

void XamlWindow::setWidth(int width) {
    this->width = width;
}

void XamlWindow::setHeight(int height) {
    this->height = height;
}

void XamlWindow::addChild(std::shared_ptr<XamlElement> child) {
    children.push_back(child);
}

std::vector<std::shared_ptr<XamlElement>> XamlWindow::getChildren() const {
    return children;
}

std::string XamlWindow::getType() const {
    return "Window";
}

void XamlWindow::render(void* platformContext) {
    // This will be implemented to create the actual window
    // using the platform-specific context
    std::cout << "Rendering Window: " << title << " (" << width << "x" << height << ")" << std::endl;
    
    // Render all children
    for (const auto& child : children) {
        child->render(platformContext);
    }
}

// ----------------------------------------
// XamlGrid Implementation
// ----------------------------------------
XamlGrid::XamlGrid() {
    setName("XamlGrid");
}

XamlGrid::~XamlGrid() = default;

void XamlGrid::addRowDefinition(const std::string& height) {
    RowDefinition rowDef;
    rowDef.height = height;
    rowDefinitions.push_back(rowDef);
}

void XamlGrid::addColumnDefinition(const std::string& width) {
    ColumnDefinition colDef;
    colDef.width = width;
    columnDefinitions.push_back(colDef);
}

void XamlGrid::addChild(std::shared_ptr<XamlElement> child) {
    children.push_back(child);
}

std::vector<std::shared_ptr<XamlElement>> XamlGrid::getChildren() const {
    return children;
}

std::string XamlGrid::getType() const {
    return "Grid";
}

void XamlGrid::render(void* platformContext) {
    std::cout << "Rendering Grid with " << rowDefinitions.size() << " rows and " 
              << columnDefinitions.size() << " columns" << std::endl;
    
    // Render all children
    for (const auto& child : children) {
        child->render(platformContext);
    }
}

// ----------------------------------------
// XamlTextBlock Implementation
// ----------------------------------------
XamlTextBlock::XamlTextBlock() : fontSize(12) {
    setName("XamlTextBlock");
}

XamlTextBlock::~XamlTextBlock() = default;

void XamlTextBlock::setText(const std::string& text) {
    this->text = text;
}

void XamlTextBlock::setFontSize(int size) {
    fontSize = size;
}

void XamlTextBlock::setFontWeight(const std::string& weight) {
    fontWeight = weight;
}

std::string XamlTextBlock::getType() const {
    return "TextBlock";
}

void XamlTextBlock::render(void* platformContext) {
    std::cout << "Rendering TextBlock: \"" << text << "\" (Font size: " << fontSize 
              << ", Weight: " << fontWeight << ")" << std::endl;
}

// ----------------------------------------
// XamlTextBox Implementation
// ----------------------------------------
XamlTextBox::XamlTextBox() : width(200) {
    setName("XamlTextBox");
}

XamlTextBox::~XamlTextBox() = default;

void XamlTextBox::setText(const std::string& text) {
    this->text = text;
}

void XamlTextBox::setWidth(int width) {
    this->width = width;
}

std::string XamlTextBox::getType() const {
    return "TextBox";
}

void XamlTextBox::render(void* platformContext) {
    std::cout << "Rendering TextBox: \"" << text << "\" (Width: " << width << ")" << std::endl;
}

// ----------------------------------------
// XamlButton Implementation
// ----------------------------------------
XamlButton::XamlButton() : width(100), height(30) {
    setName("XamlButton");
}

XamlButton::~XamlButton() = default;

void XamlButton::setContent(const std::string& content) {
    this->content = content;
}

void XamlButton::setWidth(int width) {
    this->width = width;
}

void XamlButton::setHeight(int height) {
    this->height = height;
}

void XamlButton::setClickHandler(std::function<void()> handler) {
    clickHandler = handler;
}

std::string XamlButton::getType() const {
    return "Button";
}

void XamlButton::render(void* platformContext) {
    std::cout << "Rendering Button: \"" << content << "\" (" << width << "x" << height << ")" << std::endl;
}

// ----------------------------------------
// VXamlParser Implementation
// ----------------------------------------
VXamlParser::VXamlParser() : doc(nullptr) {
    // Initialize libxml2
    xmlInitParser();
}

VXamlParser::~VXamlParser() {
    if (doc) {
        xmlFreeDoc(doc);
    }
    xmlCleanupParser();
}

std::shared_ptr<XamlWindow> VXamlParser::parseFile(const std::string& filename) {
    // Parse the XML file
    doc = xmlParseFile(filename.c_str());
    if (!doc) {
        std::cerr << "Failed to parse XAML file: " << filename << std::endl;
        return nullptr;
    }
    
    // Get the root element
    xmlNode* root = xmlDocGetRootElement(doc);
    if (!root) {
        std::cerr << "Empty XAML file" << std::endl;
        return nullptr;
    }
    
    // Parse the root element as a Window
    return parseWindow(root);
}

std::shared_ptr<XamlWindow> VXamlParser::parseString(const std::string& xaml) {
    // Parse the XML string
    doc = xmlParseMemory(xaml.c_str(), xaml.length());
    if (!doc) {
        std::cerr << "Failed to parse XAML string" << std::endl;
        return nullptr;
    }
    
    // Get the root element
    xmlNode* root = xmlDocGetRootElement(doc);
    if (!root) {
        std::cerr << "Empty XAML string" << std::endl;
        return nullptr;
    }
    
    // Parse the root element as a Window
    return parseWindow(root);
}

std::shared_ptr<XamlElement> VXamlParser::parseElement(xmlNode* node) {
    if (!node) return nullptr;
    
    // Get the element name
    std::string name = reinterpret_cast<const char*>(node->name);
    
    // Parse the element based on its name
    if (name == "Window") {
        return parseWindow(node);
    } else if (name == "Grid") {
        return parseGrid(node);
    } else if (name == "TextBlock") {
        return parseTextBlock(node);
    } else if (name == "TextBox") {
        return parseTextBox(node);
    } else if (name == "Button") {
        return parseButton(node);
    } else if (name == "Grid.RowDefinitions" || name == "Grid.ColumnDefinitions") {
        // These are handled by the Grid parser
        return nullptr;
    } else if (name == "RowDefinition" || name == "ColumnDefinition") {
        // These are handled by the Grid parser
        return nullptr;
    } else {
        std::cerr << "Unknown element: " << name << std::endl;
        return nullptr;
    }
}

std::shared_ptr<XamlWindow> VXamlParser::parseWindow(xmlNode* node) {
    auto window = std::make_shared<XamlWindow>();
    
    // Parse attributes
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        std::string name = reinterpret_cast<const char*>(attr->name);
        std::string value = reinterpret_cast<const char*>(attr->children->content);
        
        if (name == "Title") {
            window->setTitle(value);
        } else if (name == "Width") {
            window->setWidth(parseLength(value));
        } else if (name == "Height") {
            window->setHeight(parseLength(value));
        }
    }
    
    // Parse children
    for (xmlNode* child = node->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE) {
            auto element = parseElement(child);
            if (element) {
                window->addChild(element);
            }
        }
    }
    
    return window;
}

std::shared_ptr<XamlGrid> VXamlParser::parseGrid(xmlNode* node) {
    auto grid = std::make_shared<XamlGrid>();
    
    // Parse common attributes
    parseCommonAttributes(node, grid.get());
    
    // Parse children
    for (xmlNode* child = node->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE) {
            std::string name = reinterpret_cast<const char*>(child->name);
            
            if (name == "Grid.RowDefinitions") {
                // Parse row definitions
                for (xmlNode* rowDef = child->children; rowDef; rowDef = rowDef->next) {
                    if (rowDef->type == XML_ELEMENT_NODE && 
                        std::string(reinterpret_cast<const char*>(rowDef->name)) == "RowDefinition") {
                        
                        for (xmlAttr* attr = rowDef->properties; attr; attr = attr->next) {
                            std::string attrName = reinterpret_cast<const char*>(attr->name);
                            std::string attrValue = reinterpret_cast<const char*>(attr->children->content);
                            
                            if (attrName == "Height") {
                                grid->addRowDefinition(attrValue);
                            }
                        }
                    }
                }
            } else if (name == "Grid.ColumnDefinitions") {
                // Parse column definitions
                for (xmlNode* colDef = child->children; colDef; colDef = colDef->next) {
                    if (colDef->type == XML_ELEMENT_NODE && 
                        std::string(reinterpret_cast<const char*>(colDef->name)) == "ColumnDefinition") {
                        
                        for (xmlAttr* attr = colDef->properties; attr; attr = attr->next) {
                            std::string attrName = reinterpret_cast<const char*>(attr->name);
                            std::string attrValue = reinterpret_cast<const char*>(attr->children->content);
                            
                            if (attrName == "Width") {
                                grid->addColumnDefinition(attrValue);
                            }
                        }
                    }
                }
            } else {
                // Parse other elements
                auto element = parseElement(child);
                if (element) {
                    grid->addChild(element);
                }
            }
        }
    }
    
    return grid;
}

std::shared_ptr<XamlTextBlock> VXamlParser::parseTextBlock(xmlNode* node) {
    auto textBlock = std::make_shared<XamlTextBlock>();
    
    // Parse common attributes
    parseCommonAttributes(node, textBlock.get());
    
    // Parse specific attributes
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        std::string name = reinterpret_cast<const char*>(attr->name);
        std::string value = reinterpret_cast<const char*>(attr->children->content);
        
        if (name == "Text") {
            textBlock->setText(value);
        } else if (name == "FontSize") {
            textBlock->setFontSize(parseLength(value));
        } else if (name == "FontWeight") {
            textBlock->setFontWeight(value);
        }
    }
    
    return textBlock;
}

std::shared_ptr<XamlTextBox> VXamlParser::parseTextBox(xmlNode* node) {
    auto textBox = std::make_shared<XamlTextBox>();
    
    // Parse common attributes
    parseCommonAttributes(node, textBox.get());
    
    // Parse specific attributes
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        std::string name = reinterpret_cast<const char*>(attr->name);
        std::string value = reinterpret_cast<const char*>(attr->children->content);
        
        if (name == "Name") {
            textBox->setName(value);
        } else if (name == "Width") {
            textBox->setWidth(parseLength(value));
        }
    }
    
    return textBox;
}

std::shared_ptr<XamlButton> VXamlParser::parseButton(xmlNode* node) {
    auto button = std::make_shared<XamlButton>();
    
    // Parse common attributes
    parseCommonAttributes(node, button.get());
    
    // Parse specific attributes
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        std::string name = reinterpret_cast<const char*>(attr->name);
        std::string value = reinterpret_cast<const char*>(attr->children->content);
        
        if (name == "Content") {
            button->setContent(value);
        } else if (name == "Width") {
            button->setWidth(parseLength(value));
        } else if (name == "Height") {
            button->setHeight(parseLength(value));
        }
    }
    
    return button;
}

void VXamlParser::parseCommonAttributes(xmlNode* node, XamlElement* element) {
    for (xmlAttr* attr = node->properties; attr; attr = attr->next) {
        std::string name = reinterpret_cast<const char*>(attr->name);
        std::string value = reinterpret_cast<const char*>(attr->children->content);
        
        if (name == "Name") {
            element->setName(value);
        } else if (name == "Margin") {
            element->setMargin(value);
        } else if (name == "HorizontalAlignment") {
            element->setHorizontalAlignment(value);
        } else if (name == "VerticalAlignment") {
            element->setVerticalAlignment(value);
        } else if (name == "Grid.Row") {
            element->setGridRow(std::stoi(value));
        } else if (name == "Grid.Column") {
            element->setGridColumn(std::stoi(value));
        }
    }
}

int VXamlParser::parseLength(const std::string& value) {
    // Remove any non-digit characters
    std::string digits;
    for (char c : value) {
        if (std::isdigit(c)) {
            digits += c;
        }
    }
    
    return digits.empty() ? 0 : std::stoi(digits);
}

std::string VXamlParser::parseStringValue(const std::string& value) {
    return value;
}
