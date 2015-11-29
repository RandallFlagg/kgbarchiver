namespace kgb2compressor {
    namespace ROOT {
        namespace CIMV2 {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Management;
    using namespace System::Collections;
    using namespace System::Globalization;
    using namespace System::ComponentModel::Design::Serialization;
    using namespace System::Reflection;
    
    using namespace System;
    ref class Desktop;
    
    
    // Funkcje ShouldSerialize<NazwaWłaściwości> są funkcjami używanymi przez przeglądarkę właściwości programu VS do sprawdzania, czy określona właściwość ma być serializowana. Są one dodawane dla wszystkich właściwości ValueType (właściwości typu Int32, BOOL itp., które nie mogą mieć wartości pustej). Funkcje te korzystają z funkcji Is<PropertyName>Null. Są one także używane w implementacji TypeConverter dla właściwości do sprawdzania, czy istnieją wartości NULL właściwości, dzięki czemu pusta wartość może być pokazana w przeglądarce właściwości w przypadku korzystania z funkcji przeciągania i upuszczania w środowisku programu Visual Studio.
    // Funkcje typu Is<NazwaWłaściwości>Null() są używane do sprawdzania, czy właściwość ma wartość NULL.
    // Funkcje Reset<PropertyName> są dodawane dla właściwości Nullable Read/Write. Funkcje te są używane przez projektanta programu VS w przeglądarce właściwości do ustawiania dla właściwości wartości NULL.
    // Każda właściwość dodawana do klasy dla właściwości usługi WMI ma ustawione atrybuty definiujące jej zachowanie w projektancie programu Visual Studio oraz definiujące element TypeConverter, który ma być używany.
    // Klasa Elary Bound wygenerowana dla klasy usługi WMI.Win32_Desktop
    public ref class Desktop : public System::ComponentModel::Component {
        public : ref class DesktopCollection;
        public : ref class WMIValueTypeConverter;
        public : ref class ManagementSystemProperties;
        
        // Właściwość prywatna do przechowywania obszaru nazw usługi WMI, w którym znajduje się klasa.
        private: static System::String^  CreatedWmiNamespace = L"ROOT\\CIMV2";
        
        // Właściwość prywatna przechowująca nazwę klasy usługi WMI, która utworzyła tę klasę.
        private: static System::String^  CreatedClassName = L"Win32_Desktop";
        
        // Prywatna zmienna członka do przechowywania elementu ManagementScope, który jest używany przez różne metody.
        private: static System::Management::ManagementScope^  statMgmtScope = nullptr;
        
        private: kgb2compressor::ROOT::CIMV2::Desktop::ManagementSystemProperties^  PrivateSystemProperties;
        
        // Podstawowy obiekt lateBound usługi WMI.
        private: System::Management::ManagementObject^  PrivateLateBoundObject;
        
        // Zmienna członka przechowująca zachowanie dotyczące automatycznego przekazywania dla klasy.
        private: System::Boolean AutoCommitProp;
        
        // Zmienna prywatna przechowująca właściwość osadzoną reprezentującą wystąpienie.
        private: System::Management::ManagementBaseObject^  embeddedObj;
        
        // Aktualnie używany obiekt usługi WMI
        private: System::Management::ManagementBaseObject^  curObj;
        
        // Flaga wskazująca, czy wystąpienie jest obiektem osadzonym.
        private: System::Boolean isEmbedded;
        
        // Poniżej znajdują się różne przeciążenia konstruktorów inicjujących wystąpienie klasy za pomocą obiektu usługi WMI.
        public: Desktop();
        public: Desktop(System::String^  keyName);
        public: Desktop(System::Management::ManagementScope^  mgmtScope, System::String^  keyName);
        public: Desktop(System::Management::ManagementPath^  path, System::Management::ObjectGetOptions^  getOptions);
        public: Desktop(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path);
        public: Desktop(System::Management::ManagementPath^  path);
        public: Desktop(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path, System::Management::ObjectGetOptions^  getOptions);
        public: Desktop(System::Management::ManagementObject^  theObject);
        public: Desktop(System::Management::ManagementBaseObject^  theObject);
        // Właściwość zwraca obszar nazw klasy usługi WMI.
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::String^  OriginatingNamespace {
            System::String^  get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::String^  ManagementClassName {
            System::String^  get();
        }
        
        // Właściwość wskazująca obiekt osadzony w celu pobrania właściwości systemowych obiektu usługi WMI.
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property kgb2compressor::ROOT::CIMV2::Desktop::ManagementSystemProperties^  SystemProperties {
            kgb2compressor::ROOT::CIMV2::Desktop::ManagementSystemProperties^  get();
        }
        
        // Właściwość zwracająca podstawowy obiekt lateBound.
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Management::ManagementBaseObject^  LateBoundObject {
            System::Management::ManagementBaseObject^  get();
        }
        
        // Element ManagementScope obiektu.
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Management::ManagementScope^  Scope {
            System::Management::ManagementScope^  get();
            System::Void set(System::Management::ManagementScope^  value);
        }
        
        // Właściwość pokazująca zachowanie dotyczące przekazywania dla obiektu usługi WMI. Jeśli ma wartość true, obiekt usługi WMI zostanie automatycznie zapisany po każdej modyfikacji właściwości (po modyfikacji właściwości zostanie wywołana funkcja Put()).
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean AutoCommit {
            System::Boolean get();
            System::Void set(System::Boolean value);
        }
        
        // Element ManagementPath podstawowego obiektu usługi WMI.
        public: [Browsable(true)]
         property System::Management::ManagementPath^  Path {
            System::Management::ManagementPath^  get();
            System::Void set(System::Management::ManagementPath^  value);
        }
        
        // Właściwość publiczna zakresu statycznego używana przez różne metody.
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        static  property System::Management::ManagementScope^  StaticScope {
            System::Management::ManagementScope^  get();
            System::Void set(System::Management::ManagementScope^  value);
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsBorderWidthNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość BorderWidth określa szerokość obramowania wokół wszystkich okien z reg" 
L"ulowanymi obramowaniami.\nPrzykład: 3."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::UInt32 BorderWidth {
            System::UInt32 get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Krótki opis tekstowy (ciąg o długości jednego wiersza) obiektu CIM_Setting.")]
         property System::String^  Caption {
            System::String^  get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsCoolSwitchNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość CoolSwitch wskazuje, czy szybkie przełączanie zadań jest włączone. Szy" 
L"bkie przełączanie zadań umożliwia użytkownikowi przełączanie między oknami przy " 
L"użyciu kombinacji klawiaturowej ALT+TAB."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::Boolean CoolSwitch {
            System::Boolean get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsCursorBlinkRateNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość CursorBlinkRate wskazuje ostęp czasu pomiędzy kolejnymi mignięciami ku" 
L"rsora.\nPrzykład: 530."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::UInt32 CursorBlinkRate {
            System::UInt32 get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Tekstowy opis obiektu CIM_Setting.")]
         property System::String^  Description {
            System::String^  get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsDragFullWindowsNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość DragFullWindows wskazuje, czy zawartości okna są pokazywane wówczas, g" 
L"dy użytkownik przesuwa okno.\nWartości: PRAWDA lub FAŁSZ. Wartość właściwości rów" 
L"na PRAWDA oznacza, że zawartość okna jest wyświetlana wówczas, gdy użytkownik pr" 
L"zesuwa okno."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::Boolean DragFullWindows {
            System::Boolean get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsGridGranularityNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość GridGranularity wskazuje odstępy linii siatki, do której są wiązane ok" 
L"na na pulpicie. Ułatwia to organizowanie okien. Ostępy są zazwyczaj dostatecznie" 
L" małe i nie są zauważane przez użytkownika.\nPrzykład: 1."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::UInt32 GridGranularity {
            System::UInt32 get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsIconSpacingNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość IconSpacing określa odstępy między ikonami.\nPrzykład: 75"), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::UInt32 IconSpacing {
            System::UInt32 get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość IconTitleFaceName wskazuje nazwę czcionki używanej do wyświetlania naz" 
L"w ikon.\nPrzykład: MS San Serif.")]
         property System::String^  IconTitleFaceName {
            System::String^  get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsIconTitleSizeNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość IconTitleSize wskazuje rozmiar czcionki ikony.\nPrzykład: 9."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::UInt32 IconTitleSize {
            System::UInt32 get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsIconTitleWrapNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość IconTitleWrap wskazuje, czy tekst tytułu ikony jest zawijany do następ" 
L"nego wiersza..\nWartości: PRAWDA lub FAŁSZ. Wartość właściwości równa PRAWDA ozna" 
L"cza, że tytuł jest zawijany do następnego wiersza."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::Boolean IconTitleWrap {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość Name wskazuje nazwę identyfikującą aktualny profil pulpitu.\nPrzykład: " 
L"MainProf")]
         property System::String^  Name {
            System::String^  get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość Pattern wskazuje nazwę desenia używanego jako tło dla pulpitu.")]
         property System::String^  Pattern {
            System::String^  get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsScreenSaverActiveNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość ScreenSaverActive wskazuje, czy wygaszacz ekranu jest aktywny.\nWartośc" 
L"i: TRUE lub FALSE. Wartość TRUE wskazuje, że wygaszacz ekranu jest aktywny."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::Boolean ScreenSaverActive {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość ScreenSaverExecutable wskazuje nazwę bieżącego pliku wykonywalnego wyg" 
L"aszacza ekranu.\nPrzykład: LOGON.SCR.")]
         property System::String^  ScreenSaverExecutable {
            System::String^  get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsScreenSaverSecureNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość ScreenSaverSecure określa, czy hasło dla wygaszacza ekranu jest aktywn" 
L"e.\nWartości: TRUE lub FALSE. Jeśli wartość TRUE, to hasło wygaszacza ekranu jest" 
L" aktywne."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::Boolean ScreenSaverSecure {
            System::Boolean get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsScreenSaverTimeoutNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość ScreenSaverTimeout wskazuje upływ czasu przed uruchomieniem wygaszacza" 
L" ekranu."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::UInt32 ScreenSaverTimeout {
            System::UInt32 get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Identyfikator określający obiekt klasy CIM_Setting.")]
         property System::String^  SettingID {
            System::String^  get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość Wallpaper wskazuje nazwę pliku desenia tapety w tle pulpitu.\nPrzykład:" 
L" WINNT.BMP")]
         property System::String^  Wallpaper {
            System::String^  get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsWallpaperStretchedNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość WallpaperStretched wskazuje, czy tapeta jest rozciągnięta w celu wypeł" 
L"nienia całego ekranu.  Program Microsoft Plus! musi być zainstalowany, aby opcja" 
L" ta była dostępna.\nWartości: TRUE i FALSE. Wartość TRUE oznacza rozciągnięcie ta" 
L"pety na cały ekran, a wartość FALSE oznacza, że tapeta zachowa swoje oryginalne " 
L"wymiary w tle pulpitu."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::Boolean WallpaperStretched {
            System::Boolean get();
        }
        
        public: [Browsable(false), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
         property System::Boolean IsWallpaperTiledNull {
            System::Boolean get();
        }
        
        public: [Browsable(true), 
        DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden), 
        Description(L"Właściwość WallpaperTiled wskazuje, czy tapeta jest rozmieszczona sąsiadująco czy" 
L" jest wyśrodkowana.\nWartości: TRUE lub FALSE. Wartość TRUE oznacza, że tapeta je" 
L"st rozmieszczona sąsiadująco."), 
        TypeConverter(kgb2compressor::ROOT::CIMV2::Desktop::WMIValueTypeConverter::typeid)]
         property System::Boolean WallpaperTiled {
            System::Boolean get();
        }
        
        private: System::Boolean CheckIfProperClass(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path, 
                    System::Management::ObjectGetOptions^  OptionsParam);
        
        private: System::Boolean CheckIfProperClass(System::Management::ManagementBaseObject^  theObj);
        
        private: System::Boolean ShouldSerializeBorderWidth();
        
        private: System::Boolean ShouldSerializeCoolSwitch();
        
        private: System::Boolean ShouldSerializeCursorBlinkRate();
        
        private: System::Boolean ShouldSerializeDragFullWindows();
        
        private: System::Boolean ShouldSerializeGridGranularity();
        
        private: System::Boolean ShouldSerializeIconSpacing();
        
        private: System::Boolean ShouldSerializeIconTitleSize();
        
        private: System::Boolean ShouldSerializeIconTitleWrap();
        
        private: System::Boolean ShouldSerializeScreenSaverActive();
        
        private: System::Boolean ShouldSerializeScreenSaverSecure();
        
        private: System::Boolean ShouldSerializeScreenSaverTimeout();
        
        private: System::Boolean ShouldSerializeWallpaperStretched();
        
        private: System::Boolean ShouldSerializeWallpaperTiled();
        
        public: [Browsable(true)]
        System::Void CommitObject();
        
        public: [Browsable(true)]
        System::Void CommitObject(System::Management::PutOptions^  putOptions);
        
        private: System::Void Initialize();
        
        private: static System::String^  ConstructPath(System::String^  keyName);
        
        private: System::Void InitializeObject(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path, 
                    System::Management::ObjectGetOptions^  getOptions);
        
        // Różne przeciążenia elementu GetInstances() pomagają w wyliczaniu wystąpień klasy usługi WMI.
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances();
        
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances(System::String^  condition);
        
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances(cli::array< System::String ^  >^  selectedProperties);
        
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances(System::String^  condition, 
                    cli::array< System::String ^  >^  selectedProperties);
        
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances(System::Management::ManagementScope^  mgmtScope, 
                    System::Management::EnumerationOptions^  enumOptions);
        
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances(System::Management::ManagementScope^  mgmtScope, 
                    System::String^  condition);
        
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances(System::Management::ManagementScope^  mgmtScope, 
                    cli::array< System::String ^  >^  selectedProperties);
        
        public: static kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  GetInstances(System::Management::ManagementScope^  mgmtScope, 
                    System::String^  condition, cli::array< System::String ^  >^  selectedProperties);
        
        public: [Browsable(true)]
        static kgb2compressor::ROOT::CIMV2::Desktop^  CreateInstance();
        
        public: [Browsable(true)]
        System::Void Delete();
        
        // Implementacja modułu wyliczającego do wyliczania wystąpień klasy.
        public : ref class DesktopCollection : public System::Object, public System::Collections::ICollection {
            public : ref class DesktopEnumerator;
            
            private: ManagementObjectCollection^  privColObj;
            
            public: DesktopCollection(ManagementObjectCollection^  objCollection);
            public: virtual  property System::Int32 Count {
                System::Int32 get();
            }
            
            public: virtual  property System::Boolean IsSynchronized {
                System::Boolean get();
            }
            
            public: virtual  property System::Object^  SyncRoot {
                System::Object^  get();
            }
            
            public: virtual System::Void CopyTo(System::Array^  array, System::Int32 index);
            
            public: virtual System::Collections::IEnumerator^  GetEnumerator();
            
            public : ref class DesktopEnumerator : public System::Object, public System::Collections::IEnumerator {
                
                private: ManagementObjectCollection::ManagementObjectEnumerator^  privObjEnum;
                
                public: DesktopEnumerator(ManagementObjectCollection::ManagementObjectEnumerator^  objEnum);
                public: virtual  property System::Object^  Current {
                    System::Object^  get();
                }
                
                public: virtual System::Boolean MoveNext();
                
                public: virtual System::Void Reset();
            };
        };
        
        // Element TypeConverter obsługujący wartości puste dla właściwości ValueType
        public : ref class WMIValueTypeConverter : public System::ComponentModel::TypeConverter {
            
            private: System::ComponentModel::TypeConverter^  baseConverter;
            
            private: System::Type^  baseType;
            
            public: WMIValueTypeConverter(System::Type^  inBaseType);
            public: virtual System::Boolean CanConvertFrom(System::ComponentModel::ITypeDescriptorContext^  context, System::Type^  srcType) override;
            
            public: virtual System::Boolean CanConvertTo(System::ComponentModel::ITypeDescriptorContext^  context, System::Type^  destinationType) override;
            
            public: virtual System::Object^  ConvertFrom(System::ComponentModel::ITypeDescriptorContext^  context, System::Globalization::CultureInfo^  culture, 
                        System::Object^  __identifier(value)) override;
            
            public: virtual System::Object^  CreateInstance(System::ComponentModel::ITypeDescriptorContext^  context, System::Collections::IDictionary^  dictionary) override;
            
            public: virtual System::Boolean GetCreateInstanceSupported(System::ComponentModel::ITypeDescriptorContext^  context) override;
            
            public: virtual System::ComponentModel::PropertyDescriptorCollection^  GetProperties(System::ComponentModel::ITypeDescriptorContext^  context, 
                        System::Object^  __identifier(value), cli::array< System::Attribute^  >^  attributeVar) override;
            
            public: virtual System::Boolean GetPropertiesSupported(System::ComponentModel::ITypeDescriptorContext^  context) override;
            
            public: virtual System::ComponentModel::TypeConverter::StandardValuesCollection^  GetStandardValues(System::ComponentModel::ITypeDescriptorContext^  context) override;
            
            public: virtual System::Boolean GetStandardValuesExclusive(System::ComponentModel::ITypeDescriptorContext^  context) override;
            
            public: virtual System::Boolean GetStandardValuesSupported(System::ComponentModel::ITypeDescriptorContext^  context) override;
            
            public: virtual System::Object^  ConvertTo(System::ComponentModel::ITypeDescriptorContext^  context, System::Globalization::CultureInfo^  culture, 
                        System::Object^  __identifier(value), System::Type^  destinationType) override;
        };
        
        // Klasa osadzona reprezentująca właściwości systemowe usługi WMI.
        public : [TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
        ref class ManagementSystemProperties {
            
            private: System::Management::ManagementBaseObject^  PrivateLateBoundObject;
            
            public: ManagementSystemProperties(System::Management::ManagementBaseObject^  ManagedObject);
            public: [Browsable(true)]
             property System::Int32 GENUS {
                System::Int32 get();
            }
            
            public: [Browsable(true)]
             property System::String^  CLASS {
                System::String^  get();
            }
            
            public: [Browsable(true)]
             property System::String^  SUPERCLASS {
                System::String^  get();
            }
            
            public: [Browsable(true)]
             property System::String^  DYNASTY {
                System::String^  get();
            }
            
            public: [Browsable(true)]
             property System::String^  RELPATH {
                System::String^  get();
            }
            
            public: [Browsable(true)]
             property System::Int32 PROPERTY_COUNT {
                System::Int32 get();
            }
            
            public: [Browsable(true)]
             property cli::array< System::String^  >^  DERIVATION {
                cli::array< System::String^  >^  get();
            }
            
            public: [Browsable(true)]
             property System::String^  SERVER {
                System::String^  get();
            }
            
            public: [Browsable(true)]
             property System::String^  NAMESPACE {
                System::String^  get();
            }
            
            public: [Browsable(true)]
             property System::String^  PATH {
                System::String^  get();
            }
        };
    };
        }
    }
}
namespace kgb2compressor {
    namespace ROOT {
        namespace CIMV2 {
    
    
    inline Desktop::Desktop() {
        this->InitializeObject(nullptr, nullptr, nullptr);
    }
    
    inline Desktop::Desktop(System::String^  keyName) {
        this->InitializeObject(nullptr, (gcnew System::Management::ManagementPath(kgb2compressor::ROOT::CIMV2::Desktop::ConstructPath(keyName))), 
            nullptr);
    }
    
    inline Desktop::Desktop(System::Management::ManagementScope^  mgmtScope, System::String^  keyName) {
        this->InitializeObject((cli::safe_cast<System::Management::ManagementScope^  >(mgmtScope)), (gcnew System::Management::ManagementPath(kgb2compressor::ROOT::CIMV2::Desktop::ConstructPath(keyName))), 
            nullptr);
    }
    
    inline Desktop::Desktop(System::Management::ManagementPath^  path, System::Management::ObjectGetOptions^  getOptions) {
        this->InitializeObject(nullptr, path, getOptions);
    }
    
    inline Desktop::Desktop(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path) {
        this->InitializeObject(mgmtScope, path, nullptr);
    }
    
    inline Desktop::Desktop(System::Management::ManagementPath^  path) {
        this->InitializeObject(nullptr, path, nullptr);
    }
    
    inline Desktop::Desktop(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path, System::Management::ObjectGetOptions^  getOptions) {
        this->InitializeObject(mgmtScope, path, getOptions);
    }
    
    inline Desktop::Desktop(System::Management::ManagementObject^  theObject) {
        Initialize();
        if (CheckIfProperClass(theObject) == true) {
            PrivateLateBoundObject = theObject;
            PrivateSystemProperties = (gcnew kgb2compressor::ROOT::CIMV2::Desktop::ManagementSystemProperties(PrivateLateBoundObject));
            curObj = PrivateLateBoundObject;
        }
        else {
            throw (gcnew System::ArgumentException(L"Nazwa klasy nie jest odpowiednia."));
        }
    }
    
    inline Desktop::Desktop(System::Management::ManagementBaseObject^  theObject) {
        Initialize();
        if (CheckIfProperClass(theObject) == true) {
            embeddedObj = theObject;
            PrivateSystemProperties = (gcnew kgb2compressor::ROOT::CIMV2::Desktop::ManagementSystemProperties(theObject));
            curObj = embeddedObj;
            isEmbedded = true;
        }
        else {
            throw (gcnew System::ArgumentException(L"Nazwa klasy nie jest odpowiednia."));
        }
    }
    
    inline System::String^  Desktop::OriginatingNamespace::get() {
        return L"ROOT\\CIMV2";
    }
    
    inline System::String^  Desktop::ManagementClassName::get() {
        System::String^  strRet = CreatedClassName;
        if (curObj != nullptr) {
            if (curObj->ClassPath != nullptr) {
                strRet = (cli::safe_cast<System::String^  >(curObj[L"__CLASS"]));
                if ((strRet == nullptr) 
                            || (strRet == System::String::Empty)) {
                    strRet = CreatedClassName;
                }
            }
        }
        return strRet;
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::ManagementSystemProperties^  Desktop::SystemProperties::get() {
        return PrivateSystemProperties;
    }
    
    inline System::Management::ManagementBaseObject^  Desktop::LateBoundObject::get() {
        return curObj;
    }
    
    inline System::Management::ManagementScope^  Desktop::Scope::get() {
        if (isEmbedded == false) {
            return PrivateLateBoundObject->Scope;
        }
        else {
            return nullptr;
        }
    }
    inline System::Void Desktop::Scope::set(System::Management::ManagementScope^  value) {
        if (isEmbedded == false) {
            PrivateLateBoundObject->Scope = value;
        }
    }
    
    inline System::Boolean Desktop::AutoCommit::get() {
        return AutoCommitProp;
    }
    inline System::Void Desktop::AutoCommit::set(System::Boolean value) {
        AutoCommitProp = value;
    }
    
    inline System::Management::ManagementPath^  Desktop::Path::get() {
        if (isEmbedded == false) {
            return PrivateLateBoundObject->Path;
        }
        else {
            return nullptr;
        }
    }
    inline System::Void Desktop::Path::set(System::Management::ManagementPath^  value) {
        if (isEmbedded == false) {
            if (CheckIfProperClass(nullptr, __identifier(value), nullptr) != true) {
                throw (gcnew System::ArgumentException(L"Nazwa klasy nie jest odpowiednia."));
            }
            PrivateLateBoundObject->Path = value;
        }
    }
    
    inline System::Management::ManagementScope^  Desktop::StaticScope::get() {
        return statMgmtScope;
    }
    inline System::Void Desktop::StaticScope::set(System::Management::ManagementScope^  value) {
        statMgmtScope = value;
    }
    
    inline System::Boolean Desktop::IsBorderWidthNull::get() {
        if (curObj[L"BorderWidth"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::UInt32 Desktop::BorderWidth::get() {
        if (curObj[L"BorderWidth"] == nullptr) {
            return System::Convert::ToUInt32(0);
        }
        return (*cli::safe_cast<System::UInt32^>(curObj[L"BorderWidth"]));
    }
    
    inline System::String^  Desktop::Caption::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"Caption"]));
    }
    
    inline System::Boolean Desktop::IsCoolSwitchNull::get() {
        if (curObj[L"CoolSwitch"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::Boolean Desktop::CoolSwitch::get() {
        if (curObj[L"CoolSwitch"] == nullptr) {
            return System::Convert::ToBoolean(0);
        }
        return (*cli::safe_cast<System::Boolean^>(curObj[L"CoolSwitch"]));
    }
    
    inline System::Boolean Desktop::IsCursorBlinkRateNull::get() {
        if (curObj[L"CursorBlinkRate"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::UInt32 Desktop::CursorBlinkRate::get() {
        if (curObj[L"CursorBlinkRate"] == nullptr) {
            return System::Convert::ToUInt32(0);
        }
        return (*cli::safe_cast<System::UInt32^>(curObj[L"CursorBlinkRate"]));
    }
    
    inline System::String^  Desktop::Description::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"Description"]));
    }
    
    inline System::Boolean Desktop::IsDragFullWindowsNull::get() {
        if (curObj[L"DragFullWindows"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::Boolean Desktop::DragFullWindows::get() {
        if (curObj[L"DragFullWindows"] == nullptr) {
            return System::Convert::ToBoolean(0);
        }
        return (*cli::safe_cast<System::Boolean^>(curObj[L"DragFullWindows"]));
    }
    
    inline System::Boolean Desktop::IsGridGranularityNull::get() {
        if (curObj[L"GridGranularity"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::UInt32 Desktop::GridGranularity::get() {
        if (curObj[L"GridGranularity"] == nullptr) {
            return System::Convert::ToUInt32(0);
        }
        return (*cli::safe_cast<System::UInt32^>(curObj[L"GridGranularity"]));
    }
    
    inline System::Boolean Desktop::IsIconSpacingNull::get() {
        if (curObj[L"IconSpacing"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::UInt32 Desktop::IconSpacing::get() {
        if (curObj[L"IconSpacing"] == nullptr) {
            return System::Convert::ToUInt32(0);
        }
        return (*cli::safe_cast<System::UInt32^>(curObj[L"IconSpacing"]));
    }
    
    inline System::String^  Desktop::IconTitleFaceName::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"IconTitleFaceName"]));
    }
    
    inline System::Boolean Desktop::IsIconTitleSizeNull::get() {
        if (curObj[L"IconTitleSize"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::UInt32 Desktop::IconTitleSize::get() {
        if (curObj[L"IconTitleSize"] == nullptr) {
            return System::Convert::ToUInt32(0);
        }
        return (*cli::safe_cast<System::UInt32^>(curObj[L"IconTitleSize"]));
    }
    
    inline System::Boolean Desktop::IsIconTitleWrapNull::get() {
        if (curObj[L"IconTitleWrap"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::Boolean Desktop::IconTitleWrap::get() {
        if (curObj[L"IconTitleWrap"] == nullptr) {
            return System::Convert::ToBoolean(0);
        }
        return (*cli::safe_cast<System::Boolean^>(curObj[L"IconTitleWrap"]));
    }
    
    inline System::String^  Desktop::Name::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"Name"]));
    }
    
    inline System::String^  Desktop::Pattern::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"Pattern"]));
    }
    
    inline System::Boolean Desktop::IsScreenSaverActiveNull::get() {
        if (curObj[L"ScreenSaverActive"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::Boolean Desktop::ScreenSaverActive::get() {
        if (curObj[L"ScreenSaverActive"] == nullptr) {
            return System::Convert::ToBoolean(0);
        }
        return (*cli::safe_cast<System::Boolean^>(curObj[L"ScreenSaverActive"]));
    }
    
    inline System::String^  Desktop::ScreenSaverExecutable::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"ScreenSaverExecutable"]));
    }
    
    inline System::Boolean Desktop::IsScreenSaverSecureNull::get() {
        if (curObj[L"ScreenSaverSecure"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::Boolean Desktop::ScreenSaverSecure::get() {
        if (curObj[L"ScreenSaverSecure"] == nullptr) {
            return System::Convert::ToBoolean(0);
        }
        return (*cli::safe_cast<System::Boolean^>(curObj[L"ScreenSaverSecure"]));
    }
    
    inline System::Boolean Desktop::IsScreenSaverTimeoutNull::get() {
        if (curObj[L"ScreenSaverTimeout"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::UInt32 Desktop::ScreenSaverTimeout::get() {
        if (curObj[L"ScreenSaverTimeout"] == nullptr) {
            return System::Convert::ToUInt32(0);
        }
        return (*cli::safe_cast<System::UInt32^>(curObj[L"ScreenSaverTimeout"]));
    }
    
    inline System::String^  Desktop::SettingID::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"SettingID"]));
    }
    
    inline System::String^  Desktop::Wallpaper::get() {
        return (cli::safe_cast<System::String^  >(curObj[L"Wallpaper"]));
    }
    
    inline System::Boolean Desktop::IsWallpaperStretchedNull::get() {
        if (curObj[L"WallpaperStretched"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::Boolean Desktop::WallpaperStretched::get() {
        if (curObj[L"WallpaperStretched"] == nullptr) {
            return System::Convert::ToBoolean(0);
        }
        return (*cli::safe_cast<System::Boolean^>(curObj[L"WallpaperStretched"]));
    }
    
    inline System::Boolean Desktop::IsWallpaperTiledNull::get() {
        if (curObj[L"WallpaperTiled"] == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
    
    inline System::Boolean Desktop::WallpaperTiled::get() {
        if (curObj[L"WallpaperTiled"] == nullptr) {
            return System::Convert::ToBoolean(0);
        }
        return (*cli::safe_cast<System::Boolean^>(curObj[L"WallpaperTiled"]));
    }
    
    inline System::Boolean Desktop::CheckIfProperClass(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path, 
                System::Management::ObjectGetOptions^  OptionsParam) {
        if ((path != nullptr) 
                    && (System::String::Compare(path->ClassName, this->ManagementClassName, true, System::Globalization::CultureInfo::InvariantCulture) == 0)) {
            return true;
        }
        else {
            return CheckIfProperClass((gcnew System::Management::ManagementObject(mgmtScope, path, OptionsParam)));
        }
    }
    
    inline System::Boolean Desktop::CheckIfProperClass(System::Management::ManagementBaseObject^  theObj) {
        if ((theObj != nullptr) 
                    && (System::String::Compare((cli::safe_cast<System::String^  >(theObj[L"__CLASS"])), this->ManagementClassName, 
                        true, System::Globalization::CultureInfo::InvariantCulture) == 0)) {
            return true;
        }
        else {
            System::Array^  parentClasses = (cli::safe_cast<System::Array^  >(theObj[L"__DERIVATION"]));
            if (parentClasses != nullptr) {
                System::Int32 count = 0;
                for (                count = 0; (count < parentClasses->Length);                 count = (count + 1)) {
                    if (System::String::Compare((cli::safe_cast<System::String^  >(parentClasses->GetValue(count))), this->ManagementClassName, 
                        true, System::Globalization::CultureInfo::InvariantCulture) == 0) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeBorderWidth() {
        if (this->IsBorderWidthNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeCoolSwitch() {
        if (this->IsCoolSwitchNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeCursorBlinkRate() {
        if (this->IsCursorBlinkRateNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeDragFullWindows() {
        if (this->IsDragFullWindowsNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeGridGranularity() {
        if (this->IsGridGranularityNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeIconSpacing() {
        if (this->IsIconSpacingNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeIconTitleSize() {
        if (this->IsIconTitleSizeNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeIconTitleWrap() {
        if (this->IsIconTitleWrapNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeScreenSaverActive() {
        if (this->IsScreenSaverActiveNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeScreenSaverSecure() {
        if (this->IsScreenSaverSecureNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeScreenSaverTimeout() {
        if (this->IsScreenSaverTimeoutNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeWallpaperStretched() {
        if (this->IsWallpaperStretchedNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Boolean Desktop::ShouldSerializeWallpaperTiled() {
        if (this->IsWallpaperTiledNull == false) {
            return true;
        }
        return false;
    }
    
    inline System::Void Desktop::CommitObject() {
        if (isEmbedded == false) {
            PrivateLateBoundObject->Put();
        }
    }
    
    inline System::Void Desktop::CommitObject(System::Management::PutOptions^  putOptions) {
        if (isEmbedded == false) {
            PrivateLateBoundObject->Put(putOptions);
        }
    }
    
    inline System::Void Desktop::Initialize() {
        AutoCommitProp = true;
        isEmbedded = false;
    }
    
    inline System::String^  Desktop::ConstructPath(System::String^  keyName) {
        System::String^  strPath = L"ROOT\\CIMV2:Win32_Desktop";
        strPath = System::String::Concat(strPath, System::String::Concat(L".Name=", System::String::Concat(L"\"", System::String::Concat(keyName, 
                        L"\""))));
        return strPath;
    }
    
    inline System::Void Desktop::InitializeObject(System::Management::ManagementScope^  mgmtScope, System::Management::ManagementPath^  path, 
                System::Management::ObjectGetOptions^  getOptions) {
        Initialize();
        if (path != nullptr) {
            if (CheckIfProperClass(mgmtScope, path, getOptions) != true) {
                throw (gcnew System::ArgumentException(L"Nazwa klasy nie jest odpowiednia."));
            }
        }
        PrivateLateBoundObject = (gcnew System::Management::ManagementObject(mgmtScope, path, getOptions));
        PrivateSystemProperties = (gcnew kgb2compressor::ROOT::CIMV2::Desktop::ManagementSystemProperties(PrivateLateBoundObject));
        curObj = PrivateLateBoundObject;
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances() {
        return GetInstances(nullptr, nullptr, nullptr);
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances(System::String^  condition) {
        return GetInstances(nullptr, condition, nullptr);
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances(cli::array< System::String ^  >^  selectedProperties) {
        return GetInstances(nullptr, nullptr, selectedProperties);
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances(System::String^  condition, cli::array< System::String ^  >^  selectedProperties) {
        return GetInstances(nullptr, condition, selectedProperties);
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances(System::Management::ManagementScope^  mgmtScope, 
                System::Management::EnumerationOptions^  enumOptions) {
        if (mgmtScope == nullptr) {
            if (statMgmtScope == nullptr) {
                mgmtScope = (gcnew System::Management::ManagementScope());
                mgmtScope->Path->NamespacePath = L"root\\CIMV2";
            }
            else {
                mgmtScope = statMgmtScope;
            }
        }
        System::Management::ManagementPath^  pathObj = (gcnew System::Management::ManagementPath());
        pathObj->ClassName = L"Win32_Desktop";
        pathObj->NamespacePath = L"root\\CIMV2";
        System::Management::ManagementClass^  clsObject = (gcnew System::Management::ManagementClass(mgmtScope, pathObj, nullptr));
        if (enumOptions == nullptr) {
            enumOptions = (gcnew System::Management::EnumerationOptions());
            enumOptions->EnsureLocatable = true;
        }
        return (gcnew kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection(clsObject->GetInstances(enumOptions)));
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances(System::Management::ManagementScope^  mgmtScope, 
                System::String^  condition) {
        return GetInstances(mgmtScope, condition, nullptr);
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances(System::Management::ManagementScope^  mgmtScope, 
                cli::array< System::String ^  >^  selectedProperties) {
        return GetInstances(mgmtScope, nullptr, selectedProperties);
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection^  Desktop::GetInstances(System::Management::ManagementScope^  mgmtScope, 
                System::String^  condition, cli::array< System::String ^  >^  selectedProperties) {
        if (mgmtScope == nullptr) {
            if (statMgmtScope == nullptr) {
                mgmtScope = (gcnew System::Management::ManagementScope());
                mgmtScope->Path->NamespacePath = L"root\\CIMV2";
            }
            else {
                mgmtScope = statMgmtScope;
            }
        }
        System::Management::ManagementObjectSearcher^  ObjectSearcher = (gcnew System::Management::ManagementObjectSearcher(mgmtScope, 
            (gcnew SelectQuery(L"Win32_Desktop", condition, selectedProperties))));
        System::Management::EnumerationOptions^  enumOptions = (gcnew System::Management::EnumerationOptions());
        enumOptions->EnsureLocatable = true;
        ObjectSearcher->Options = enumOptions;
        return (gcnew kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection(ObjectSearcher->Get()));
    }
    
    inline kgb2compressor::ROOT::CIMV2::Desktop^  Desktop::CreateInstance() {
        System::Management::ManagementScope^  mgmtScope = nullptr;
        if (statMgmtScope == nullptr) {
            mgmtScope = (gcnew System::Management::ManagementScope());
            mgmtScope->Path->NamespacePath = CreatedWmiNamespace;
        }
        else {
            mgmtScope = statMgmtScope;
        }
        System::Management::ManagementPath^  mgmtPath = (gcnew System::Management::ManagementPath(CreatedClassName));
        System::Management::ManagementClass^  tmpMgmtClass = (gcnew System::Management::ManagementClass(mgmtScope, mgmtPath, nullptr));
        return (gcnew kgb2compressor::ROOT::CIMV2::Desktop(tmpMgmtClass->CreateInstance()));
    }
    
    inline System::Void Desktop::Delete() {
        PrivateLateBoundObject->Delete();
    }
    
    
    inline Desktop::DesktopCollection::DesktopCollection(ManagementObjectCollection^  objCollection) {
        privColObj = objCollection;
    }
    
    inline System::Int32 Desktop::DesktopCollection::Count::get() {
        return privColObj->Count;
    }
    
    inline System::Boolean Desktop::DesktopCollection::IsSynchronized::get() {
        return privColObj->IsSynchronized;
    }
    
    inline System::Object^  Desktop::DesktopCollection::SyncRoot::get() {
        return this;
    }
    
    inline System::Void Desktop::DesktopCollection::CopyTo(System::Array^  array, System::Int32 index) {
        privColObj->CopyTo(array, index);
        System::Int32 nCtr;
        for (        nCtr = 0; (nCtr < array->Length);         nCtr = (nCtr + 1)) {
            array->SetValue((gcnew kgb2compressor::ROOT::CIMV2::Desktop((cli::safe_cast<System::Management::ManagementObject^  >(array->GetValue(nCtr))))), 
                nCtr);
        }
    }
    
    inline System::Collections::IEnumerator^  Desktop::DesktopCollection::GetEnumerator() {
        return (gcnew kgb2compressor::ROOT::CIMV2::Desktop::DesktopCollection::DesktopEnumerator(privColObj->GetEnumerator()));
    }
    
    
    inline Desktop::DesktopCollection::DesktopEnumerator::DesktopEnumerator(ManagementObjectCollection::ManagementObjectEnumerator^  objEnum) {
        privObjEnum = objEnum;
    }
    
    inline System::Object^  Desktop::DesktopCollection::DesktopEnumerator::Current::get() {
        return (gcnew kgb2compressor::ROOT::CIMV2::Desktop((cli::safe_cast<System::Management::ManagementObject^  >(privObjEnum->Current))));
    }
    
    inline System::Boolean Desktop::DesktopCollection::DesktopEnumerator::MoveNext() {
        return privObjEnum->MoveNext();
    }
    
    inline System::Void Desktop::DesktopCollection::DesktopEnumerator::Reset() {
        privObjEnum->Reset();
    }
    
    
    inline Desktop::WMIValueTypeConverter::WMIValueTypeConverter(System::Type^  inBaseType) {
        baseConverter = System::ComponentModel::TypeDescriptor::GetConverter(inBaseType);
        baseType = inBaseType;
    }
    
    inline System::Boolean Desktop::WMIValueTypeConverter::CanConvertFrom(System::ComponentModel::ITypeDescriptorContext^  context, 
                System::Type^  srcType) {
        return baseConverter->CanConvertFrom(context, srcType);
    }
    
    inline System::Boolean Desktop::WMIValueTypeConverter::CanConvertTo(System::ComponentModel::ITypeDescriptorContext^  context, 
                System::Type^  destinationType) {
        return baseConverter->CanConvertTo(context, destinationType);
    }
    
    inline System::Object^  Desktop::WMIValueTypeConverter::ConvertFrom(System::ComponentModel::ITypeDescriptorContext^  context, 
                System::Globalization::CultureInfo^  culture, System::Object^  __identifier(value)) {
        return baseConverter->ConvertFrom(context, culture, __identifier(value));
    }
    
    inline System::Object^  Desktop::WMIValueTypeConverter::CreateInstance(System::ComponentModel::ITypeDescriptorContext^  context, 
                System::Collections::IDictionary^  dictionary) {
        return baseConverter->CreateInstance(context, dictionary);
    }
    
    inline System::Boolean Desktop::WMIValueTypeConverter::GetCreateInstanceSupported(System::ComponentModel::ITypeDescriptorContext^  context) {
        return baseConverter->GetCreateInstanceSupported(context);
    }
    
    inline System::ComponentModel::PropertyDescriptorCollection^  Desktop::WMIValueTypeConverter::GetProperties(System::ComponentModel::ITypeDescriptorContext^  context, 
                System::Object^  __identifier(value), cli::array< System::Attribute^  >^  attributeVar) {
        return baseConverter->GetProperties(context, __identifier(value), attributeVar);
    }
    
    inline System::Boolean Desktop::WMIValueTypeConverter::GetPropertiesSupported(System::ComponentModel::ITypeDescriptorContext^  context) {
        return baseConverter->GetPropertiesSupported(context);
    }
    
    inline System::ComponentModel::TypeConverter::StandardValuesCollection^  Desktop::WMIValueTypeConverter::GetStandardValues(
                System::ComponentModel::ITypeDescriptorContext^  context) {
        return baseConverter->GetStandardValues(context);
    }
    
    inline System::Boolean Desktop::WMIValueTypeConverter::GetStandardValuesExclusive(System::ComponentModel::ITypeDescriptorContext^  context) {
        return baseConverter->GetStandardValuesExclusive(context);
    }
    
    inline System::Boolean Desktop::WMIValueTypeConverter::GetStandardValuesSupported(System::ComponentModel::ITypeDescriptorContext^  context) {
        return baseConverter->GetStandardValuesSupported(context);
    }
    
    inline System::Object^  Desktop::WMIValueTypeConverter::ConvertTo(System::ComponentModel::ITypeDescriptorContext^  context, 
                System::Globalization::CultureInfo^  culture, System::Object^  __identifier(value), System::Type^  destinationType) {
        if (baseType->BaseType == System::Enum::typeid) {
            if (__identifier(value)->GetType() == destinationType) {
                return __identifier(value);
            }
            if (((__identifier(value) == nullptr) 
                        && (context != nullptr)) 
                        && (context->PropertyDescriptor->ShouldSerializeValue(context->Instance) == false)) {
                return  "NULL_ENUM_VALUE" ;
            }
            return baseConverter->ConvertTo(context, culture, __identifier(value), destinationType);
        }
        if ((baseType == System::Boolean::typeid) 
                    && (baseType->BaseType == System::ValueType::typeid)) {
            if (((__identifier(value) == nullptr) 
                        && (context != nullptr)) 
                        && (context->PropertyDescriptor->ShouldSerializeValue(context->Instance) == false)) {
                return L"";
            }
            return baseConverter->ConvertTo(context, culture, __identifier(value), destinationType);
        }
        if ((context != nullptr) 
                    && (context->PropertyDescriptor->ShouldSerializeValue(context->Instance) == false)) {
            return L"";
        }
        return baseConverter->ConvertTo(context, culture, __identifier(value), destinationType);
    }
    
    
    inline Desktop::ManagementSystemProperties::ManagementSystemProperties(System::Management::ManagementBaseObject^  ManagedObject) {
        PrivateLateBoundObject = ManagedObject;
    }
    
    inline System::Int32 Desktop::ManagementSystemProperties::GENUS::get() {
        return (*cli::safe_cast<System::Int32^>(PrivateLateBoundObject[L"__GENUS"]));
    }
    
    inline System::String^  Desktop::ManagementSystemProperties::CLASS::get() {
        return (cli::safe_cast<System::String^  >(PrivateLateBoundObject[L"__CLASS"]));
    }
    
    inline System::String^  Desktop::ManagementSystemProperties::SUPERCLASS::get() {
        return (cli::safe_cast<System::String^  >(PrivateLateBoundObject[L"__SUPERCLASS"]));
    }
    
    inline System::String^  Desktop::ManagementSystemProperties::DYNASTY::get() {
        return (cli::safe_cast<System::String^  >(PrivateLateBoundObject[L"__DYNASTY"]));
    }
    
    inline System::String^  Desktop::ManagementSystemProperties::RELPATH::get() {
        return (cli::safe_cast<System::String^  >(PrivateLateBoundObject[L"__RELPATH"]));
    }
    
    inline System::Int32 Desktop::ManagementSystemProperties::PROPERTY_COUNT::get() {
        return (*cli::safe_cast<System::Int32^>(PrivateLateBoundObject[L"__PROPERTY_COUNT"]));
    }
    
    inline cli::array< System::String^  >^  Desktop::ManagementSystemProperties::DERIVATION::get() {
        return (cli::safe_cast<cli::array< System::String^  >^  >(PrivateLateBoundObject[L"__DERIVATION"]));
    }
    
    inline System::String^  Desktop::ManagementSystemProperties::SERVER::get() {
        return (cli::safe_cast<System::String^  >(PrivateLateBoundObject[L"__SERVER"]));
    }
    
    inline System::String^  Desktop::ManagementSystemProperties::NAMESPACE::get() {
        return (cli::safe_cast<System::String^  >(PrivateLateBoundObject[L"__NAMESPACE"]));
    }
    
    inline System::String^  Desktop::ManagementSystemProperties::PATH::get() {
        return (cli::safe_cast<System::String^  >(PrivateLateBoundObject[L"__PATH"]));
    }
        }
    }
}
