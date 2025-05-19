#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <functional>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <VString>
#include <VObject>

// Forward declarations
class XamlElement;
class XamlWindow;
class XamlGrid;
class XamlTextBlock;
class XamlTextBox;
class XamlButton;

// Base class for all XAML elements
class XamlElement : public VObject {
public:
    XamlElement();
    virtual ~XamlElement();

    // Common properties for all elements
    void setName(const std::string& name);
    std::string getName() const;
    
    void setMargin(const std::string& margin);
    void setHorizontalAlignment(const std::string& alignment);
    void setVerticalAlignment(const std::string& alignment);
    
    // Grid-specific properties
    void setGridRow(int row);
    void setGridColumn(int column);
    
    // Get element type
    virtual std::string getType() const = 0;
    
    // Render the element (platform-specific)
    virtual void render(void* platformContext) = 0;

protected:
    std::string elementName;
    std::string margin;
    std::string horizontalAlignment;
    std::string verticalAlignment;
    int gridRow;
    int gridColumn;
};

// Window element
class XamlWindow : public XamlElement {
public:
    XamlWindow();
    ~XamlWindow() override;
    
    void setTitle(const std::string& title);
    void setWidth(int width);
    void setHeight(int height);
    
    // Add getter methods
    const std::string& getTitle() const { return title; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    void addChild(std::shared_ptr<XamlElement> child);
    std::vector<std::shared_ptr<XamlElement>> getChildren() const;
    
    std::string getType() const override;
    void render(void* platformContext) override;
    
private:
    std::string title;
    int width;
    int height;
    std::vector<std::shared_ptr<XamlElement>> children;
};

// Grid element
class XamlGrid : public XamlElement {
public:
    XamlGrid();
    ~XamlGrid() override;
    
    void addRowDefinition(const std::string& height);
    void addColumnDefinition(const std::string& width);
    
    void addChild(std::shared_ptr<XamlElement> child);
    std::vector<std::shared_ptr<XamlElement>> getChildren() const;
    
    std::string getType() const override;
    void render(void* platformContext) override;
    
private:
    struct RowDefinition {
        std::string height;
    };
    
    struct ColumnDefinition {
        std::string width;
    };
    
    std::vector<RowDefinition> rowDefinitions;
    std::vector<ColumnDefinition> columnDefinitions;
    std::vector<std::shared_ptr<XamlElement>> children;
};

// TextBlock element
class XamlTextBlock : public XamlElement {
public:
    XamlTextBlock();
    ~XamlTextBlock() override;
    
    void setText(const std::string& text);
    void setFontSize(int size);
    void setFontWeight(const std::string& weight);
    
    const std::string& getText() const { return text; }
    int getFontSize() const { return fontSize; }
    const std::string& getFontWeight() const { return fontWeight; }
    
    std::string getType() const override;
    void render(void* platformContext) override;
    
private:
    std::string text;
    int fontSize;
    std::string fontWeight;
};

// TextBox element
class XamlTextBox : public XamlElement {
public:
    XamlTextBox();
    ~XamlTextBox() override;
    
    void setText(const std::string& text);
    void setWidth(int width);
    
    const std::string& getText() const { return text; }
    int getWidth() const { return width; }
    
    std::string getType() const override;
    void render(void* platformContext) override;
    
private:
    std::string text;
    int width;
};

// Button element
class XamlButton : public XamlElement {
public:
    XamlButton();
    ~XamlButton() override;
    
    void setContent(const std::string& content);
    void setWidth(int width);
    void setHeight(int height);
    
    void setClickHandler(std::function<void()> handler);
    
    const std::string& getContent() const { return content; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    std::string getType() const override;
    void render(void* platformContext) override;
    
private:
    std::string content;
    int width;
    int height;
    std::function<void()> clickHandler;
};

// XAML Parser class
class VXamlParser {
public:
    VXamlParser();
    ~VXamlParser();
    
    // Parse a XAML file and create a window
    std::shared_ptr<XamlWindow> parseFile(const std::string& filename);
    
    // Parse a XAML string and create a window
    std::shared_ptr<XamlWindow> parseString(const std::string& xaml);
    
private:
    // Helper methods for parsing
    std::shared_ptr<XamlElement> parseElement(xmlNode* node);
    std::shared_ptr<XamlWindow> parseWindow(xmlNode* node);
    std::shared_ptr<XamlGrid> parseGrid(xmlNode* node);
    std::shared_ptr<XamlTextBlock> parseTextBlock(xmlNode* node);
    std::shared_ptr<XamlTextBox> parseTextBox(xmlNode* node);
    std::shared_ptr<XamlButton> parseButton(xmlNode* node);
    
    // Helper methods for parsing attributes
    void parseCommonAttributes(xmlNode* node, XamlElement* element);
    void parseGridAttributes(xmlNode* node, XamlElement* element);
    
    // Helper methods for parsing values
    int parseLength(const std::string& value);
    std::string parseStringValue(const std::string& value);
    
    // XML document
    xmlDoc* doc;
};
