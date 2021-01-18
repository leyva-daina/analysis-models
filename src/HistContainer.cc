#include "TFile.h"
#include "Analysis/Models/interface/HistContainer.h"
#include "Analysis/Models/interface/Tools.h"


using namespace analysis::models;

HistContainer::HistContainer(const std::string& input) : histFileName_(input) {
  data_ = getHistogram_("data");
}

HistContainer::~HistContainer() = default;

void HistContainer::Rebin(const int& n) const {
	if(data_) data_->Rebin(n);
}

std::unique_ptr<TH1> HistContainer::data() const {
  return uniqueClone_(*data_);
}


std::unique_ptr<TH1> HistContainer::bbH() const {
  return uniqueClone_(*bbH_);
}


std::unique_ptr<TH1> HistContainer::background() const {
  return uniqueClone_(*summedBackground_);
}


std::vector<std::unique_ptr<TH1> > HistContainer::backgrounds() const {
  std::vector<std::unique_ptr<TH1> > result;
  for (const auto& bkg: backgroundTemplates_) {
    result.emplace_back(uniqueClone_(*bkg));
  }
  return result;
}


void HistContainer::show() const {
  if(data_) std::cout << "Data events:            " << data_->Integral() << std::endl;
}


std::unique_ptr<TH1> HistContainer::getHistogram_(const std::string& name) const {
  TFile file(histFileName_.c_str(), "read");
  //std::string dir_name = "general";  		//add if there is directory
  std::string hist_name = "mbb";
  //if(name == "data") hist_name = "diJet_m";	//old version
  //TDirectory *dir = (TDirectory*)file.Get((dir_name).c_str());
  //if(!file.Get((dir_name).c_str())) return nullptr;
  std::unique_ptr<TH1> hist = staticCastUnique<TH1>(file.Get((hist_name).c_str())->Clone(name.c_str()));
  if(!hist) return nullptr;

  hist->Rebin(50); 
  hist->SetDirectory(0);
  file.Close();
  if (hist->GetSumw2N() == 0) hist->Sumw2();
  return hist;
}


std::unique_ptr<TH1> HistContainer::uniqueClone_(const TH1& original) {
  std::unique_ptr<TH1> clone(static_cast<TH1*>(uniqueClone(&original)));
  if (clone != nullptr) clone->SetDirectory(0);
  return clone;
}
