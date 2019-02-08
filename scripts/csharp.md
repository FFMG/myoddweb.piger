# C Sharp Example

## But is is just a powershell script isn't it ...

Yes, yes it is

```ps1
$source = @"
using Am;

// Hello World! program
namespace Foo
{
  public class Bar {
    static public void Boo( Core am )
    {
      am.Say( "Hello world", 3000, 5 );
      System.Console.WriteLine("Hello World again!");
    }
  }
}
"@

$assem = (
  "AMPowerShellCmdLets.dll" 
   ) 

Add-Type -TypeDefinition $source -ReferencedAssemblies $assem -Language CSharp
[Foo.Bar]::Boo( $am )

Write-Host "Press any key to continue ..."
[void]($host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown"))
```