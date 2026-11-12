import { docsifyConfig } from './config.js';
import { updateRouteClasses } from '../core/routes.js';
import { initShell } from '../shell.js';
import { configureRunner } from '../playground/backend.js';

// This module must run before Docsify and its plugins evaluate.
window.$docsify = docsifyConfig;
configureRunner();
updateRouteClasses();
initShell();
