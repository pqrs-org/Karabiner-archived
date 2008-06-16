void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;

  RemapUtil::jis_toggle_eisuu_kana(params, %%PARAMS%%);
}
