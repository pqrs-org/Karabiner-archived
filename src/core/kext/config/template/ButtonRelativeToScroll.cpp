void
%%LASTNAME%%(const RemapPointingParams_relative &params)
{
  if (! config.%%LASTNAME%%) return;

  static ButtonRelativeToScroll brts;
  brts.remap(params, %%PARAMS%%);
}
