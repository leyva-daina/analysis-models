#ifndef Analysis_Models_FitContainer_h
#define Analysis_Models_FitContainer_h 1

#include <vector>
#include <string>
#include <memory>
#include <array>
#include <fstream>

#include "TH1.h"
#include "TCanvas.h"

#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"

#include "Analysis/Models/interface/HistContainer.h"
#include "Analysis/Models/interface/TreeContainer.h"
#include "Analysis/Models/interface/ParamModifier.h"


namespace analysis {
  namespace models {

    class FitContainer {
    public:
      enum class Type { signal, background };
      inline static std::string toString(const Type& type) {
        switch (type) {
        case Type::signal: return "signal";
        case Type::background: return "background";
        };
        return "";              // to silence compiler
      };

      //FitContainer(const TH1* data, const TH1* signal, const TH1* background, const std::string& outputDir);
      FitContainer(const TH1* data, const std::string& outputDir, const std::string & type = "data");
      FitContainer(TTree& data, const std::string& outputDir, const std::string& dataleaf = "mbb", const std::string & weightleaf = "weight");
      FitContainer(const HistContainer& container, const std::string& outputDir);
      FitContainer(const TreeContainer& container, const std::string& outputDir);
      virtual ~FitContainer();
      void initialize();

      FitContainer(const FitContainer&) = default;
      FitContainer& operator=(const FitContainer&) = default;
      FitContainer(FitContainer&&) = default;
      FitContainer& operator=(FitContainer&&) = default;

      FitContainer& verbosity(int level);
      FitContainer& fitRangeMin(float min);
      FitContainer& fitRangeMax(float max);
      FitContainer& setNBins(int nbins);
      RooWorkspace& getWorkspace();

      void setModel(const Type& type, const std::string& model);
      void setModel(const Type& type, const std::string& model, const std::vector<ParamModifier>& modifiers);
      std::unique_ptr<RooFitResult> modelFit(const std::string& model, const bool & weighted = false, const bool& plot_params = 0);
      std::unique_ptr<RooFitResult> FitSignal(const std::string & model, const bool& plot_params = 0);

      void profileModel(const Type& type);
      void showModels() const;
      void Import(const RooAbsArg& inArg);
      void Write();

    private:

      //Private constructor to avoid code duplication for private members initialisation
      FitContainer(const std::string& outputDir);

      // methods to set the fit model
      double getPeakStart_(const Type& type,const double& max);
      double getPeakStart_(const Type& type);
      double getMaxPosition_(const RooAbsData& data);

      // internal methods
      static void prepareCanvas_(TCanvas& raw);
      static void prepareFrame_(RooPlot& raw);
      std::string getOutputPath_(const std::string& subdirectory = "");
      int getNonZeroBins_(const RooAbsData& data);
      int getBlindedBins_(const RooAbsData& data, double blind_lowEdge, double blind_highEdge);
      bool applyModifiers_(RooAbsPdf& pdf, const std::vector<ParamModifier>& modifiers);
      void makeLog_(const RooFitResult& fitResult);

      // data member
      bool initialized_;
      bool written_;
      bool splitrange_;
      std::string outputDir_;
      std::string plotDir_;
      std::string workspaceDir_;
      std::string fullRangeId_;
      std::string fitRangeId_;
      std::string fitRangeLowId_;        // for split range by CA
      //std::string fitRangeMedId_;       // for split range by CA
      std::string fitRangeHighId_;        // for split range by CA
      std::string fitSplRangeId_;        // for split range by CA
      double blind_lowEdge_;
      double blind_highEdge_;
      int verbosity_;
      RooWorkspace workspace_;
      std::string outRootFileName_;
      std::string mbb_;
      std::string weight_;
      std::string data_;
      std::string signal_;
      std::string bkg_;
      float fitRangeMin_;
      float fitRangeMax_;
      TTree modelFit_;
      float chi2BkgOnly_;
      float normChi2BkgOnly_;
      int ndfBkgOnly_;
      double covMatrix_[400];
      double eigenVector_[400];
      int nbins_;
      float lumi_;
      float obs_;
      std::string type_;
      bool weighted_;
    };

    inline void FitContainer::Import(const RooAbsArg& inArg){ workspace_.import(inArg);}
    inline void FitContainer::Write(){ if(!written_) { workspace_.writeToFile(outRootFileName_.c_str()); written_ = true;}   }
    inline RooWorkspace& FitContainer::getWorkspace() {return workspace_;};

  }
}

#endif  // Analysis_Models_FitContainer_h
