#include "Auctioneer.h"

Auctioneer::Auctioneer() : Person{}, auctionDate{"None"} {}

Auctioneer::Auctioneer(const std::string& name, const std::string& birthDate, const std::string& auctionDate)
        : Person(name, birthDate), auctionDate(auctionDate) {}

Auctioneer::Auctioneer(const std::string& name, const std::string& birthDate, const std::string& auctionDate,
                       const std::map<std::string, std::pair<std::string, double>>& auctionedPaintings)
        : Person(name, birthDate), auctionDate(auctionDate), auctionedPaintings(auctionedPaintings) {}

Auctioneer::Auctioneer(const Auctioneer &other)
    : Person(other), auctionedPaintings(other.auctionedPaintings), auctionDate(other.auctionDate) {}

Auctioneer::Auctioneer(Auctioneer &&other) noexcept
        : Person(std::move(other)), auctionedPaintings(std::move(other.auctionedPaintings)), auctionDate(std::move(other.auctionDate)) {}


Auctioneer& Auctioneer::operator=(const Auctioneer& other) {
    if (this == &other) return *this;
    Person::operator=(other);
    auctionedPaintings = other.auctionedPaintings;
    auctionDate = other.auctionDate;
    return *this;
}

Auctioneer& Auctioneer::operator=(Auctioneer&& other) noexcept {
    if (this == &other) return *this;
    Person::operator=(std::move(other));
    auctionedPaintings = std::move(other.auctionedPaintings);
    auctionDate = std::move(other.auctionDate);
    return *this;
}

Auctioneer::~Auctioneer() {}


void Auctioneer::addAuctionedPainting(const std::string& painting, const std::string& artistName, double price) {
    std::regex latinRegex("^[A-Za-z\\s]+$");
    if (!std::regex_match(painting, latinRegex) || !std::regex_match(artistName, latinRegex)) {
        std::cout << "Invalid painting or artist name. Please use only Latin letters and spaces.\n";
        return;
    }
    if (price <= 0) {
        std::cout << "Price must be greater than zero. Please try again." << std::endl;
        return;
    }
    auctionedPaintings[painting] = std::make_pair(artistName, price);
}

std::string Auctioneer::getAuctionDate() const {
    return auctionDate;
}

void Auctioneer::setAuctionDate(const std::string &auctionDate) {
    this->auctionDate = auctionDate;
}

std::map<std::string, std::pair<std::string,double>> Auctioneer::getAuctionedPaintings() const {
    return auctionedPaintings;
}

void Auctioneer::addAuctioneer(std::vector<Auctioneer>& auctioneers) {
    while (true) {
        try {
            Auctioneer auctioneer;
            auctioneer.input();
            auctioneers.push_back(std::move(auctioneer));
            break;
        } catch (...) {
            std::cout << "An error occurred while adding the auctioneer. Please try again." << std::endl;
        }
    }
}

void Auctioneer::input() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Person::input();

    // Regex for validating date in YYYY-MM-DD format
    std::regex dateRegex("^\\d{4}-\\d{2}-\\d{2}$");

    // Input for auction date with validation
    while (true) {
        try {
            std::cout << "Enter auction date (YYYY-MM-DD): ";
            std::getline(std::cin, auctionDate);
            if (auctionDate.empty() || !std::regex_match(auctionDate, dateRegex)) {
                throw std::runtime_error("Invalid auction date format.");
            }
            break;
        } catch (...) {
            std::cout << "Invalid input. Please enter the date in YYYY-MM-DD format." << std::endl;
        }
    }

    // Input for auctioned paintings
    int numPaintings;
    std::cout << "Enter the number of paintings to be auctioned: ";
    while (!(std::cin >> numPaintings) || numPaintings < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a positive integer for the number of paintings: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auctionedPaintings.clear(); // Clear existing entries in case of re-input

    // Loop to input each painting's details
    std::regex latinRegex("^[A-Za-z\\s]+$");
    for (int i = 0; i < numPaintings; ++i) {
        std::string paintingTitle, artistName;
        double price;

        // Input and validate painting title
        while (true) {
            std::cout << "Enter the title for painting #" << (i + 1) << " (Latin letters only): ";
            std::getline(std::cin, paintingTitle);
            if (std::regex_match(paintingTitle, latinRegex)) {
                break;
            }
            std::cout << "Invalid input. Please use only Latin letters and spaces for the title.\n";
        }

        // Input and validate artist name
        while (true) {
            std::cout << "Enter the artist name for painting #" << (i + 1) << " (Latin letters only): ";
            std::getline(std::cin, artistName);
            if (std::regex_match(artistName, latinRegex)) {
                break;
            }
            std::cout << "Invalid input. Please use only Latin letters and spaces for the artist name.\n";
        }

        // Input and validate price
        std::cout << "Enter the price for painting #" << (i + 1) << ": ";
        while (!(std::cin >> price) || price <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a positive price: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Add the painting to the map
        auctionedPaintings[paintingTitle] = std::make_pair(artistName, price);
    }
}

void Auctioneer::displayInfo() const {
    std::cout << "Auctioneer: " << getName() << "\n";
    std::cout << "Birthday: " << getBirthDate() << "\n";
    std::cout << "Auction date: " << auctionDate << "\n";
    std::cout << "Auctioned paintings: \n";

    if (auctionedPaintings.empty()) {
        std::cout << "No paintings are currently being auctioned.\n";
    } else {
        for (const auto& pair : auctionedPaintings) {
            const std::string& paintingTitle = pair.first;
            const std::string& artistName = pair.second.first;
            double price = pair.second.second;

            std::cout << paintingTitle << " by " << artistName << ", $" << price << "\n";
        } std::cout << std::endl;
    }
}


void Auctioneer::sellPainting(const std::string& painting) {
    auto it = auctionedPaintings.find(painting);
    if (it == auctionedPaintings.end()) {
        std::cout << "Painting not found in auction. Please try another title." << std::endl;
    } else {
        auctionedPaintings.erase(it);
        std::cout << "Painting sold: " << painting << std::endl;
    }
}

void Auctioneer::sortAuctioneersByName(std::vector<Auctioneer>& auctioneers) {
    if (auctioneers.empty()) {
        std::cout << "No auctioneers to sort." << std::endl;
        return;
    }

    std::sort(auctioneers.begin(), auctioneers.end(), [](const Auctioneer& a, const Auctioneer& b) {
        return a.getName() < b.getName();
    });
    for (const auto& auctioneer : auctioneers) auctioneer.displayInfo();
}

void Auctioneer::holdAuction(Painting* painting, std::vector<Collector*>& collectors) {
    if (painting == nullptr) {
        std::cout << "Painting does not exist. Please check the painting details." << std::endl;
        return;
    }

    std::cout << "Auction for: " << painting->getTitle() << "\n";
    int highestBid = 0;
    Collector *highestBidder = nullptr;

    for (auto &collector: collectors) {
        int bid = collector->placeBid(painting);
        if (bid > highestBid) {
            highestBid = bid;
            highestBidder = collector;
        }
        if (highestBidder) {
            highestBidder->purchasePainting(painting, highestBid);
            std::cout << highestBidder->getName() << " won the auction for $" << highestBid << "!\n";
        } else {
            std::cout << "No bids were placed.\n";
        }
    }
}

void Auctioneer::getDataFromObject(std::ostream &os) const {
    Person::getDataFromObject(os);

    for (const auto &entry : auctionedPaintings) {
        const std::string &painting = entry.first;
        const std::string &artistName = entry.second.first;
        double price = entry.second.second;

        os << painting << std::endl;
        os << artistName << std::endl;
        os << price << std::endl;
    }

    os << "AUCTION_DATE:" << std::endl;
    os << auctionDate << std::endl;
}

void Auctioneer::setDataToObject(std::istream &is) {
    Person::setDataToObject(is);  // Load base Person data

    auctionedPaintings.clear();
    std::string painting;
    std::string artistName;
    double price;

    // Read each painting's details until "AUCTION_DATE:" label is found
    while (std::getline(is, painting)) {
        if (painting == "AUCTION_DATE:") {
            break;
        }

        std::getline(is, artistName);
        is >> price;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore any trailing newline

        auctionedPaintings[painting] = std::make_pair(artistName, price);
    }

    // Read the auction date
    std::getline(is, auctionDate);
}

void Auctioneer::loadAuctioneers(std::vector<Auctioneer> &auctioneers) {
    std::ifstream file(AUCTIONEER_FILE);
    if (!file.is_open()) {
        std::cerr << "Error opening auctioneer file\n";
        return;
    }

    auctioneers.clear();
    Auctioneer auctioneer;
    while (true) {
        auctioneer.setDataToObject(file);
        if (file.fail()) {
            break;
        }
        auctioneers.push_back(auctioneer);
    }
    file.close();
}

void Auctioneer::saveAuctioneers(const std::vector<Auctioneer> &auctioneers) {
    std::ofstream file(AUCTIONEER_FILE);
    for (const auto& auctioneer : auctioneers) {
        auctioneer.getDataFromObject(file);
    }
    file.close();
}