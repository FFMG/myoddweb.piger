namespace ActionMonitor.Shell.Runners
{
  internal interface IRunner
  {
    /// <summary>
    /// The unique Id given to us by the caller.
    /// </summary>
    string Uuid { get; }
  }
}
